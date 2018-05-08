/* Copyright (c) 2018, Aleksandrov Maksim */

#ifndef CWATER_OPENCL
#define CWATER_OPENCL

#include <vector>

/**
 * @brief A class that performs parallel processing of data.
 */
class CWaterOpenCL {
public:
    /*
     * @constructor
     * @breaf default constructor.
     * @detail In case of an error, it throws exceptions to int.
     *         These exceptions apply to OpenCL.
     */
    CWaterOpenCL();

    /*
     * @destructor
     */
    ~CWaterOpenCL();

    /*
     * @brief Loading data into buffers.
     * @detail Loads data into the buffers of the computing device. 
     *         It also calculates additional required data.
     * @param vertex Vertex data.
     * @param faces Faces data.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int loadData(const std::vector <float> &vertex,
                 const std::vector <uint32_t> &faces);

    /*
     * @brief Preparation of internal polygons.
     * @param faces Result buffer.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int getInnerFaces(std::vector <uint32_t> &faces) const;

    /*
     * @brief Preparation of border polygons.
     * @param faces Result buffer.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int getBorderFaces(std::vector <uint32_t> &faces) const;

    /*
     * @brief Get edges fracture.
     * @param edges Result buffer.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int getFractureEdges(std::vector <uint32_t> &edges) const;

    /*
     * @brief Get inner edges.
     * @param edges Result buffer.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int getBorderEdges(std::vector <uint32_t> &edges) const;

    /*
     * @brief Get border edges.
     * @param edges Result buffer.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int getInnerEdges(std::vector <uint32_t> &edges) const;
private:
    // To store OpenCL pointers.
    typedef void* CLdescriptor;

    class CKernel;

    /*
     * Class for managing OpenCL objects.
     */
    class CMemObject {
    public:
        /*
         * @constructor
         */
        CMemObject();

        /*
         * @constructor
         * @detail In case of an error, it throws exceptions to int.
         *         These exceptions apply to OpenCL.
         * @param context The context for which the memory object is created.
         * @param size The size of the memory object.
         * @param flag Object flag.
         */
        CMemObject(const CLdescriptor context,
                   const uint32_t size,
                   const int flag);
        /*
         * @destructor
         */
        ~CMemObject();
        
        /*
         * @brief Loading data into an object.
         * @param context The context for which the memory object is created.
         * @param commandQueue The command queue for loading data.
         * @param data Pointer to data.
         * @param size Size of data.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int loadData(const CLdescriptor context,
                     const CLdescriptor commandQueue,
                     const char *data,
                     const uint32_t size);

        /*
         * @brief Get data from object.
         * @param commandQueue  The command queue for uploading data.
         * @param data Output buffer.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int getData(const CLdescriptor commandQueue,
                    std::vector<float> &data) const;

        /*
         * @brief Get data from object.
         * @param commandQueue  The command queue for uploading data.
         * @param data Output buffer.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int getData(const CLdescriptor commandQueue,
                    std::vector<uint32_t> &data) const;

        /*
         * @brief Get data from object.
         * @param commandQueue  The command queue for uploading data.
         * @param data Output buffer.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int getData(const CLdescriptor commandQueue,
                    uint32_t &data) const;

        /*
         * @brief Get size of memory object.
         * @return Size in bytes.
         */
        uint32_t getSize() const;

        /*
         * @brief Set flag of memory object.
         * @detail If you specify a flag that differs from the current object, 
         *         it will be recreated. All data will be lost.
         * @param context The context for which the memory object is created.
         * @param flag New flag.
         * @return 0 if sucsses, else OpenCl error code..
         */
        int setFlag(const CLdescriptor context,
                    const int flag);

        /*
         * @brief Set new size of memory object.
         * @detail If you specify a size that differs from the current object, 
         *         it will be recreated. All data will be lost.
         * @param context The context for which the memory object is created.
         * @param size New size.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int resize(const CLdescriptor context,
                   const uint32_t size);
        friend CKernel;
    private:
        /*
         * TODO 
         */
        CMemObject & operator= (const CMemObject & sec);

        /*
         * @brief Get data from object.
         * @detail To prevent memory leaks, delete the received ptr via delete.
         * @param commandQueue  The command queue for uploading data.
         * @param data Output buffer.
         * @param size Size of returned data in bytes.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int getData(const CLdescriptor commandQueue,
                    char **data,
                    uint32_t &size) const;
        CLdescriptor mem_;
        uint32_t size_;
        int flag_;
    };

    /*
     * A class for controlling the bound function.
     */
    class CKernel {
    public:
        /*
         * @constructor
         */
        CKernel();

        /*
         * @destructor
         */
        ~CKernel();

        /*
         * @brief Function attachment.
         * @detail Must be pre-set parameters.
         * @param program Compiled program.
         * @param name Char string. Function name.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int setFunction(const CLdescriptor program,
                        const char *name);

        /*
         * @brief Bind parametr to function.
         * @param memObject Memory object to parametr.
         * @param paramId Position of parametr.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int bindParametr(const CMemObject &memObject,
                         const uint32_t paramId);

        /*
         * @brief Complite function.
         * @param commandQueue The command queue for uploading data.
         * @param workSizes Count iteration array. workSizes[0] * workSizes[1] *...
         * @param countWorkSizes Size of workSizes.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int complite(const CLdescriptor commandQueue,
                     const uint32_t *workSizes,
                     const uint32_t countWorkSizes);
    private:
        CLdescriptor kernel_;
    };

    /*
     * @brief Calculation of additional data.
     * @detail Should be called after loading vertices and polygons.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int computeData();

    /*
     * @brief Init nessary kernels.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int initKernels();

    /*
     * @brief Calculation inside the lying polygons.
     * @detail The result is filling the data with the following changes:
     *         bufferInnerEdges_, bufferInnerEdgeFaces_, countInnerEdges_.
     * @param bufferEdges Buffer with data on the edges.
     * @param bufferMarkNoneBorder A buffer with the notation that the 
     *                             edge is already inside. It is necessary 
     *                             for the calculation of edges - boundaries.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int computeInnerEdges(const CMemObject &bufferEdges,
                          const CMemObject &bufferMarkNoneBorder);

    /*
     * @brief Calculation of edges on the edge.
     * @detail The result is filling the data with the following changes:
     *         bufferBorderEdges_, bufferBorderEdgeFaces_, countBorderEdges_.
     * @param bufferEdges Buffer with data on the edges.
     * @param bufferMarkNoneBorder A buffer with the notation that the 
     *                             edge is already inside.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int computeBorderEdges(const CMemObject &bufferEdges,
                           const CMemObject &bufferMarkNoneBorder);
    /*
     * @brief Calculation of fracture edges.
     * @detail The result is filling the data with the following changes:
     *         bufferFractureEdges_, countFractureEdges_.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int computeFractureEdges();

    CMemObject bufferVertex_,
               bufferFaces_,
               bufferInnerEdges_,
               bufferInnerEdgeFaces_,
               bufferBorderEdges_,
               bufferBorderEdgeFaces_,
               // ids inner edges
               bufferFractureEdges_;

    CKernel kernelFindEdges,
            kernelFindInnerEdges,
            kernelFindBorderEdges,
            kernelFindFractureEdges;

    uint32_t countInnerEdges_ = 0,
             countBorderEdges_ = 0,
             countFractureEdges_ = 0;

    CLdescriptor program_ = nullptr,
                 commandQueue_ = nullptr,
                 context_ = nullptr;
};



#endif // CWATER_OPENCL