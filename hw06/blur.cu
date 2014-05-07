#include "reference_calc.cpp"
  #include "utils.h"
 
    __global__
  void gaussian_blur(const unsigned char* const inputChannel,
                     unsigned char* const outputChannel,
                     int numRows, int numCols,
                     const float* const filter, const int filterWidth)
  {
 
    const int pixel_col = blockIdx.x;
    const int pixel_row = threadIdx.x;
 
    int offset_pixel_row, offset_pixel_col;
 
    // iterate through the kernel
    float acc = 0.f;
    for (int col=0; col<filterWidth; col++) {
      for (int row=0; row<filterWidth; row++) {
        offset_pixel_row = pixel_row - filterWidth/2 + row;
        offset_pixel_col = pixel_col - filterWidth/2 + col;
        // clamp to the image boundary
        if (offset_pixel_row < 0) {
          offset_pixel_row = 0;
        } else if (offset_pixel_row > numRows - 1) {
          offset_pixel_row = numRows - 1;
        }   
        if (offset_pixel_col < 0) {
          offset_pixel_col = 0;
        } else if (offset_pixel_col > numCols - 1) {
          offset_pixel_col = numCols - 1;
        }
        acc += filter[filterWidth * row + col] * inputChannel[offset_pixel_row * numCols + offset_pixel_col];
      }
    }
    // assign the 1D output channel the accumulated val
    outputChannel[pixel_row * numCols + pixel_col] = (unsigned char)((int)acc);
  }
 
  //This kernel takes in an image represented as a uchar4 and splits
  //it into three images consisting of only one color channel each
  __global__
  void separateChannels(const uchar4* const inputImageRGBA,
                        int numRows,
                        int numCols,
                        unsigned char* const redChannel,
                        unsigned char* const greenChannel,
                        unsigned char* const blueChannel)
  {
    const int pixel_col = blockIdx.x;
    const int pixel_row = threadIdx.x;
    const int i = pixel_row * numCols + pixel_col;
    redChannel[i] = inputImageRGBA[i].x;
    greenChannel[i] = inputImageRGBA[i].y;
    blueChannel[i] = inputImageRGBA[i].z;
  }
 
  //This kernel takes in three color channels and recombines them
  //into one image.  The alpha channel is set to 255 to represent
  //that this image has no transparency.
  __global__
  void recombineChannels(const unsigned char* const redChannel,
                         const unsigned char* const greenChannel,
                         const unsigned char* const blueChannel,
                         uchar4* const outputImageRGBA,
                         int numRows,
                         int numCols)
  {
    const int pixel_col = blockIdx.x;
    const int pixel_row = threadIdx.x;
    const int thread_1D_pos = pixel_row * numCols + pixel_col;
 
    unsigned char red   = redChannel[thread_1D_pos];
    unsigned char green = greenChannel[thread_1D_pos];
    unsigned char blue  = blueChannel[thread_1D_pos];
 
    //Alpha should be 255 for no transparency
    uchar4 outputPixel = make_uchar4(red, green, blue, 255);
 
    outputImageRGBA[thread_1D_pos] = outputPixel;
  }
 
  unsigned char *d_red, *d_green, *d_blue;
  float         *d_filter;
 
  void allocateMemoryAndCopyToGPU(const size_t numRowsImage, const size_t numColsImage,
                                  const float* const h_filter, const size_t filterWidth)
  {
 
    //allocate memory for the three different channels
    checkCudaErrors(cudaMalloc(&d_red,   sizeof(unsigned char) * numRowsImage * numColsImage));
    checkCudaErrors(cudaMalloc(&d_green, sizeof(unsigned char) * numRowsImage * numColsImage));
    checkCudaErrors(cudaMalloc(&d_blue,  sizeof(unsigned char) * numRowsImage * numColsImage));
 
    // Allocate memory for the filter on the GPU
    checkCudaErrors(cudaMalloc(&d_filter, sizeof(float) * filterWidth * filterWidth));
 
    // Copy the filter on the host (h_filter) to the memory you just allocated
    checkCudaErrors(cudaMemcpy(d_filter, h_filter, sizeof(float) * filterWidth * filterWidth, cudaMemcpyHostToDevice));
 
  }
 
  void your_gaussian_blur(const uchar4 * const h_inputImageRGBA, uchar4 * const d_inputImageRGBA,
                          uchar4* const d_outputImageRGBA, const size_t numRows, const size_t numCols,
                          unsigned char *d_redBlurred,
                          unsigned char *d_greenBlurred,
                          unsigned char *d_blueBlurred,
                          const int filterWidth)
  {
    const dim3 blockSize = numRows; //n rows, means n threads per col
    const dim3 gridSize = numCols;  //m cols, means m blocks with n rows (threads) each
 
    separateChannels<<<gridSize, blockSize>>>(d_inputImageRGBA,
                                              numRows,
                                              numCols,
                                              d_red,
                                              d_green,
                                              d_blue);
 
    cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());
 
    // perform blurring on each channel
    gaussian_blur<<<gridSize, blockSize>>>(d_red, d_redBlurred, numRows, numCols, d_filter, filterWidth);
    gaussian_blur<<<gridSize, blockSize>>>(d_green, d_greenBlurred, numRows, numCols, d_filter, filterWidth);
    gaussian_blur<<<gridSize, blockSize>>>(d_blue, d_blueBlurred, numRows, numCols, d_filter, filterWidth);
 
    cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());
 
    recombineChannels<<<gridSize, blockSize>>>(d_redBlurred,
                                               d_greenBlurred,
                                               d_blueBlurred,
                                               d_outputImageRGBA,
                                               numRows,
                                               numCols);
    cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());
  }
 
 
  // free the red, green, and blue channels we allocated
  void cleanup() {
    checkCudaErrors(cudaFree(d_red));
    checkCudaErrors(cudaFree(d_green));
    checkCudaErrors(cudaFree(d_blue));
  }