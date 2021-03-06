/* Copyright (c) 2018, Aleksandrov Maksim */

#ifndef CWATER_OPENCL
#define CWATER_OPENCL

#include <vector>
#include <list>

/**
 * A class that performs parallel processing of data.
 */
class CWaterOpenCL {
public:
    /*
     * @constructor
     * @breaf default constructor.
     * @details In case of an error, it throws exceptions to int.
     *         These exceptions apply to OpenCL.
     */
    CWaterOpenCL();

    /*
     * @destructor
     */
    ~CWaterOpenCL();

    /*
     * Loading data into buffers.
     * @details Loads data into the buffers of the computing device. 
     *         It also calculates additional required data.
     * @param vertex Vertex data.
     * @param faces Faces data.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int loadData(const std::vector <float> &vertex,
                 const std::vector <uint32_t> &faces);

    /*
     * Returns an array with the id of the polygons 
     *        adjacent to the inner edges_.
     * @param faces Result buffer.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int getInnerEdgesFaces(std::vector <uint32_t> &faces) const;

    /*
     * Preparation of border polygons.
     * @param faces Result buffer.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int getBorderFaces(std::vector <uint32_t> &faces) const;

    /*
     * Get edges_ fracture.
     * @param edges_ Result buffer.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int getFractureEdges(std::vector <uint32_t> &edges) const;

    /*
     * Get inner edges_.
     * @param edges_ Result buffer.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int getBorderEdges(std::vector <uint32_t> &edges) const;

    int getVertex(std::vector <float> &result) const;

protected:
    /**
     * Cleaning of internal data.
     */
    void clearOpenCl();

    /*
     * Search for internal tank polygons.
     * @param borders Data on the boundaries of capacities.
     * @param faces Result buffer.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int computeInnerFaces(const std::list<std::vector<uint32_t>> &borders, 
                          std::list<std::vector<uint32_t>> &faces);

    int computeAreaData(const std::list<std::vector<uint32_t>>& areas,
                        const std::vector<float> &heights,
                        std::vector<float> &resultSquare,
                        std::vector<float> &resultVal);


    /*
     * Search for stacking.
     * @param areas Polygons related to tanks.
     * @param newAreas New capacitances formed by intersection.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int removeCommunityAreas(std::list<std::vector<uint32_t>> &areas,
                             std::list<std::vector<uint32_t>> &newAreas);

    int findUnionVertex(const std::list<std::vector<uint32_t>> &areas,
                        std::list<std::vector<uint32_t>> &unionVertex);

    int getFacesToHeight(const std::vector<uint32_t> &area,
                         const float                  height,
                               std::vector<uint32_t> &faces, 
                               std::vector<float>    &vertex);

    std::vector <uint32_t> facesIn_,
                           edgesHoleBorders_;
    uint32_t countInnerEdges_ = 0,
             countBorderEdges_ = 0,
             countFractureEdges_ = 0;
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
         * @details In case of an error, it throws exceptions to int.
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

        int concat(const CLdescriptor context,
                   const CLdescriptor commandQueue,
                   const CMemObject &obj1,
                   const CMemObject &obj2);

        /*
         * Loading data into an object.
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
         * Get data from object.
         * @param commandQueue  The command queue for uploading data.
         * @param data Output buffer.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int getData(const CLdescriptor commandQueue,
                    std::vector<float> &data) const;
        /*
         * Get data from object.
         * @param commandQueue  The command queue for uploading data.
         * @param data Output buffer.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int getData(const CLdescriptor commandQueue,
                    std::vector<bool> &data) const;

        /*
         * Get data from object.
         * @param commandQueue  The command queue for uploading data.
         * @param data Output buffer.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int getData(const CLdescriptor commandQueue,
                    std::vector<uint32_t> &data) const;

        /*
         * Get data from object.
         * @param commandQueue  The command queue for uploading data.
         * @param data Output buffer.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int getData(const CLdescriptor commandQueue,
                    uint32_t &data) const;

        /*
         * Get data from object.
         * @param commandQueue  The command queue for uploading data.
         * @param data Output buffer.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int getData(const CLdescriptor commandQueue,
                    std::vector<char> &data) const;

        /*
         * Get size of memory object.
         * @return Size in bytes.
         */
        uint32_t getSize() const;

        /*
         * Set flag of memory object.
         * @details If you specify a flag that differs from the current object, 
         *         it will be recreated. All data will be lost.
         * @param context The context for which the memory object is created.
         * @param flag New flag.
         * @return 0 if sucsses, else OpenCl error code..
         */
        int setFlag(const CLdescriptor context,
                    const int flag);

        void swap(CMemObject &second);

        /*
         * Set new size of memory object.
         * @details If you specify a size that differs from the current object, 
         *         it will be recreated.
         * @param context The context for which the memory object is created.
         * @param commandQueue The command queue for transfer data.
         * @param size New size.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int resize(const CLdescriptor context,
                   const CLdescriptor commandQueue,
                   const uint32_t size);

        /*
        * Set new size of memory object.
        * @details If you specify a size that differs from the current object, 
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
         * Get data from object.
         * @details To prevent memory leaks, delete the received ptr via delete.
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
         * Function attachment.
         * @details Must be pre-set parameters.
         * @param program Compiled program.
         * @param name Char string. Function name.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int setFunction(const CLdescriptor program,
                        const char *name);

        /*
         * Bind parametr to function.
         * @param memObject Memory object to parametr.
         * @param paramId Position of parametr.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int bindParametr(const CMemObject &memObject,
                         const uint32_t paramId) const;

        /*
         * Bind parametr to function.
         * @param param Uint parametr.
         * @param paramId Position of parametr.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int bindParametr(const uint32_t param,
                         const uint32_t paramId) const;
        /*
         * Bind parametr to function.
         * @param param Float parametr.
         * @param paramId Position of parametr.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int bindParametr(const float param,
                         const uint32_t paramId) const;
        /*
         * Complite function.
         * @param commandQueue The command queue for uploading data.
         * @param workSizes Count iteration array. workSizes[0] * workSizes[1] *...
         * @param countWorkSizes Size of workSizes.
         * @return 0 if sucsses, else OpenCl error code.
         */
        int complite(const CLdescriptor commandQueue,
                     const uint32_t *workSizes,
                     const uint32_t countWorkSizes) const;
    private:
        CLdescriptor kernel_;
    };

    /*
     * Calculation of additional data.
     * @details Should be called after loading vertices and polygons.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int computeData();

    /*
     * Init nessary kernels.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int initKernels();

    /*
     * Calculation inside the lying polygons.
     * @details The result is filling the data with the following changes: countInnerEdges_.
     * @param bufferEdges Buffer with data on the edges_.
     * @param bufferMarkNoneBorder A buffer with the notation that the 
     *                             edge is already inside. It is necessary 
     *                             for the calculation of edges_ - boundaries.
     * @param bufferInnerEdges The buffer storing the resulting internal edges_.
     * @param bufferInnerFaces The buffer storing the resulting internal faces.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int computeInnerEdges(const CMemObject &bufferEdges,
                          const CMemObject &bufferMarkNoneBorder,
                          const CMemObject &bufferInnerEdges,
                          const CMemObject &bufferInnerFaces);

    /*
     * Calculation of edges_ on the edge.
     * @details The result is filling the data with the following changes:
     *         edgesHoleBorders_, countBorderEdges_.
     * @param bufferEdges Buffer with data on the edges_.
     * @param bufferMarkNoneBorder A buffer with the notation that the 
     *                             edge is already inside.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int computeBorderEdges(const CMemObject &bufferEdges,
                           const CMemObject &bufferMarkNoneBorder);

    /*
     * Calculation of fracture edges_.
     * @details The result is filling the data with the following changes:
     *         edgesHoleBorders_, countFractureEdges_.
     * @param bufferInnerFaces Buffer with information about the senetrained polygons.
     * @param bufferInnerEdges Buffer with information about the senetrained edges_.
     * @return 0 if sucsses, else OpenCl error code.
     */
    int computeFractureEdges(const CMemObject &bufferInnerFaces,
                             const CMemObject &bufferInnerEdges);

    CMemObject bufferVertex_,
               bufferFaces_;

    CKernel kernelDeleteDoubleVert_,
            kernelFindEdges_,
            kernelFindInnerEdges_,
            kernelFindBorderEdges_,
            kernelFindFractureEdges_,
            kernelFindInnerFaces_,
            kernelCountColise_,
            kernelRemoveCommunityAreas_,
            kernelFindUnionVertex_,
            kernelGetMarkData_,
            kernelFindVal_,
            kernelFindSquare_,
            kernelGetFacesToHeight_;

    CLdescriptor program_ = nullptr,
                 commandQueue_ = nullptr,
                 context_ = nullptr;
};



#endif // CWATER_OPENCL