/** *****************************************************************************
 *   @copyright :  Copyright (C) 2022 Qin ZhaoYu. All rights reserved.
 *
 *   @author    :  Qin ZhaoYu.
 *   @brief     :  To Get started with the HDF5 framework.
 *
 *   To note that dll must be loaded while running hdf5(implicit load used here).
 *
 *   About HDF5 file structure：
 *   +-- /
 *   |   +-- group_1           # Groups, like folders, each HDF5 file is actually the root group '/'.
 *   |   |   +-- dataset_1_1   # DataSets, including two parts: raw data values and metadata.
 *   |   |   |   +-- (Raw) Datas    (eg: a 4 x 5 x 6 matrix)
 *   |   |   |   +-- Metadata
 *   |   |   |   |   +-- Dataspace  (eg: Rank = 3, Dimensions = {4, 5, 6}) # give the `rank` and `dimension` of raw data
 *   |   |   |   |   +-- Datatype   (eg: Integer)                          # give the `data type`
 *   |   |   |   |   +-- Properties (eg: Chuncked, Compressed)             # describe the block storage and compression
 *   |   |   |   |   +-- Attributes (eg: attr1 = 32.4, attr2 = "hello")    # other custom properties of the dataset
 *   |   |   |   +-- ...
 *   |   |   +-- ...
 *   |   |
 *   |   +-- ...
 *   |
 *
 *   API description:
 *       + H5 : Library common functions
 *       + H5A: Annotation api for accessing attributes and operating routines.
 *       + H5D: Dataset    api for operating dataset.
 *       + H5E: Error      api for handling error routines.
 *       + H5F: File       api for opening and saving files.
 *       + H5G: Group      api for creating and running groups.
 *       + H5I: Identification api for identifying routines.
 *       + H5P: Property   api for operating property.
 *       + H5R: Reference  api for referencing routines.
 *       + H5S: Data size  api for defining and accessing data routines.
 *       + H5T: Type       api for creating and operating data type.
 *       + H5Z: Zipping    api for compressing data.
 *
 *   see : https://blog.csdn.net/david830_wu/article/details/63782190
 *         https://zhuanlan.zhihu.com/p/104145585
 *         https://portal.hdfgroup.org/display/HDF5/HDF5
 *
 *   Change History:
 *   -----------------------------------------------------------------------------
 *   v1.0, 2022/02/14, Qin ZhaoYu,
 *   Init model.
 *
 ** ******************************************************************************/
#include "hdf5.h"

void hdf5_file_create_close()
{
    printf("=================== hdf5 usages\n");

    char *file_name = "file.h5";
    hid_t file_id; // hid_t 是HDF5对象id的通用数据类型，每个id标志一个HDF5对象。
    herr_t status; // herr_t 是HDF5报错和状态的通用数据类型（通常执行失败返回负数，成功返回非负数0）。

    // hid_t H5Fcreate(
    //     const char *filename,  文件名
    //     unsigned int flags,    是否覆盖原文件（H5F_ACC_TRUNC，能覆盖）
    //     hid_t create_plist,    属性列表
    //     hid_t access_plist)    访问属性列表
    file_id = H5Fcreate(file_name, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    status = H5Fclose(file_id);
    printf("hdf5 file closed after created, status: %d\n", status);

    // hid_t H5Fopen(
    //     const char *filename,  文件名
    //     unsigned int flags,    文件读写属性（H5F_ACC_RDWR，可读写；H5F_ACC_RDONLY，只读）
    //     hid_t access_plist)    访问属性列表
    file_id = H5Fopen(file_name, H5F_ACC_RDWR, H5P_DEFAULT);
    status = H5Fclose(file_id);
    printf("hdf5 file closed after opened, status: %d\n", status);
}

void hdf5_group_create_close()
{
    char *file_name = "file.h5";
    hid_t file_id = H5Fopen(file_name, H5F_ACC_RDWR, H5P_DEFAULT); // 打开 hdf5 文件。
    herr_t status;

    // hid_t H5Gcreate2(
    //     hid_t loc_id,      父group（file_id 也相当于一个父group）
    //     const char *name,  绝对/相对链接名
    //     hid_t lcpl_id,     链接性质
    //     hid_t gcpl_id,     组创建性质
    //     hid_t gapl_id)     组访问性质
    hid_t group1 = H5Gcreate(file_id, "MyGroup1", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    hid_t group2 = H5Gcreate(group1, "./MyGroup2", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    /*
    +-- /
    |   +-- group "MyGroup1"
    |   |   +-- group "MyGroup2"
    |   |   |   + None
    */

    // herr_t H5Gclose(hid_t group_id)
    status = H5Gclose(group2);
    status = H5Gclose(group1);
    printf("group closed after created, status: %d\n", status);

    // hid_t H5Gopen2(
    //     hid_t loc_id,      父group
    //     const char *name,  绝对/相对链接名
    //     hid_t gapl_id)     组访问性质
    group2 = H5Gopen(file_id, "MyGroup1/MyGroup2", H5P_DEFAULT);
    status = H5Gclose(group2);
    printf("group closed after opened, status: %d\n", status);

    status = H5Fclose(file_id); // 关闭 hdf5 文件。
}

void hdf5_group_check()
{
    char *file_name = "file.h5";
    hid_t file_id = H5Fopen(file_name, H5F_ACC_RDWR, H5P_DEFAULT);
    herr_t status;

    // herr_t H5Eset_auto2(
    //     hid_t estack_id,    要设置的错误堆栈
    //     H5E_auto2_t func,   遇到错误时调用的函数将错误打印，默认H5Eprint2
    //     void *client_data)  传递给错误处理函数的数据，默认是标准错误流stderr
    status = H5Eset_auto(H5E_DEFAULT, NULL, NULL); // 关闭错误堆栈自动打印功能

    // herr_t H5Lget_info(
    //     hid_t loc_id,       父group
    //     const char *name,   组名称
    //     H5L_info_t *linfo,  组的信息结构体buffer
    //     hid_t lapl_id)      组的访问性质列表
    status = H5Lget_info(file_id, "MyGroup1/MyGroup2", NULL, H5P_DEFAULT); // 获取组信息
    printf("check group \"MyGroup1/MyGroup2\": ");
    if (status == 0)
        printf("existed\n");
    else
        printf("not existed\n");

    status = H5Lget_info(file_id, "MyGroup3", NULL, H5P_DEFAULT);
    printf("check group \"MyGroup3\": ");
    if (status == 0)
        printf("existed\n");
    else
        printf("not existed\n");

    status = H5Eset_auto(H5E_DEFAULT, (H5E_auto2_t)H5Eprint2, stderr); // 开启堆栈错误打印
    status = H5Fclose(file_id);
}

void generate_matrix(int rows, int columns, double **&matrix, int type = 1)
{
    double *data_mem = new double[rows * columns];
    matrix = new double *[rows]; // 修改指针地址，传指针引用。

    for (int i = 0; i < rows; ++i)
    {
        // 为矩阵分配内存，这里通过一维数组分配连续内存提高效率。
        matrix[i] = data_mem + i * columns;
    }

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < columns; ++c)
        {
            if (type == 1)
                matrix[r][c] = r + 0.01 * c;
            else
                matrix[r][c] = 0;
        }
    }
}

void hdf5_dataset_create_write()
{
    char *file_name = "file.h5";
    hid_t file_id = H5Fopen(file_name, H5F_ACC_RDWR, H5P_DEFAULT);
    herr_t status;

    int rows = 10, columns = 8;
    double **matrix;
    generate_matrix(rows, columns, matrix);

    unsigned int rank = 2;
    hsize_t dims[2];
    dims[0] = rows;
    dims[1] = columns;

    /*dataspace一共有三种形式:
        + H5S_SCALAR : 标量，只有一个元素，rank为0;
        + H5S_SIMPLE：正常的数组形式的元素;
        + H5S_NULL：没有数据元素;
    */

    // hid_t  H5Screate_simple(
    //     int rank,                 空间维度
    //     const hsize_t dims[],     每个维度元素个数（为0则无法写入数据）
    //     const hsize_t maxdims[]); 每个维度元素个数上限（为NULL则和当前dim相同）
    hid_t dataspace_id = H5Screate_simple(rank, dims, NULL); // 创建数据集metadata中dataspace信息项目

    // hid_t H5Dcreate2(
    //     hid_t loc_id,       hdf5中位置id
    //     const char *name,   数据集名称
    //     hid_t type_id,      数据类型
    //     hid_t space_id,     数据空间（dataspace）id
    //     hid_t lcpl_id,      链接创建属性
    //     hid_t dcpl_id,      数据集创建属性
    //     hid_t dapl_id)      数据集访问属性
    hid_t dataset_id = H5Dcreate(file_id, "MyGroup1/dset", H5T_NATIVE_DOUBLE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT,
                                 H5P_DEFAULT); // 创建数据集中的数据本身。

    printf("dataspace and dataset created.\n");

    // herr_t H5Dwrite(
    //     hid_t dset_id,      写入目标数据集id
    //     hid_t mem_type_id,  内存数据格式
    //     hid_t mem_space_id, 内存数据空间（H5S_ALL文件数据空间用于内存数据空间）
    //     hid_t file_space_id,文件数据空间（H5S_ALL由数据集的当前维度定义的文件中数据空间）
    //     hid_t plist_id,     IO操作类型
    //     const void *buf)    待使用数据缓冲区
    status = H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, matrix[0]); // 将数据写入数据集。
    printf("dataset writed.\n");

    /*file.h5 数据结构（>>>h5dump.exe file.h5）：
    +-- /
    |   +-- group: "MyGroup1"
    |   |   +-- group: "MyGroup2"
    |   |   |   +-- None
    |   |   +-- DataSet: "dset"
    |   |   |   +-- DataType: H5T_IEEE_F64LE
    |   |   |   +-- DataSpace: Simple ((10, 8)/(10, 8))
    |   |   |   +-- Data:
    |   |   |       (0,0): 0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07,
    |   |   |       (1,0): 1, 1.01, 1.02, 1.03, 1.04, 1.05, 1.06, 1.07,
    |   |   |       (2,0): 2, 2.01, 2.02, 2.03, 2.04, 2.05, 2.06, 2.07,
    |   |   |       (3,0): 3, 3.01, 3.02, 3.03, 3.04, 3.05, 3.06, 3.07,
    |   |   |       (4,0): 4, 4.01, 4.02, 4.03, 4.04, 4.05, 4.06, 4.07,
    |   |   |       (5,0): 5, 5.01, 5.02, 5.03, 5.04, 5.05, 5.06, 5.07,
    |   |   |       (6,0): 6, 6.01, 6.02, 6.03, 6.04, 6.05, 6.06, 6.07,
    |   |   |       (7,0): 7, 7.01, 7.02, 7.03, 7.04, 7.05, 7.06, 7.07,
    |   |   |       (8,0): 8, 8.01, 8.02, 8.03, 8.04, 8.05, 8.06, 8.07,
    |   |   |       (9,0): 9, 9.01, 9.02, 9.03, 9.04, 9.05, 9.06, 9.07
    |   |   |
    */

    status = H5Dclose(dataset_id); // no order asked here.
    status = H5Sclose(dataspace_id);
    printf("dataset and dataspace closed.\n");

    status = H5Fclose(file_id);
    delete[] matrix; // matrix 和 data_mem 使用的是同一份内存。
}

void print_matrix_2d(int rows, int columns, double **matrix)
{
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < columns; ++c)
            printf("%f  ", matrix[r][c]);
        printf("\n");
    }
}

void hdf5_dataset_read()
{
    char *file_name = "file.h5";
    hid_t file_id = H5Fopen(file_name, H5F_ACC_RDWR, H5P_DEFAULT);
    herr_t status;

    int rows = 10, columns = 8;
    double **matrix;
    generate_matrix(rows, columns, matrix, 0);

    // hid_t H5Dopen2(
    //     hid_t file_id,    数据集所在Group的父Group的id
    //     const char *name, 数据集名称
    //     hid_t dapl_id)    数据集访问性质
    hid_t dataset_id = H5Dopen(file_id, "MyGroup1/dset", H5P_DEFAULT);

    // 提取内存数据类型相关信息。
    hid_t datatype = H5Dget_type(dataset_id);        // datatype handler, 数据类型处理器。
    H5T_class_t type_class = H5Tget_class(datatype); // 数据类型，整数、浮点数、字符串、
    H5T_order_t data_order = H5Tget_order(datatype); // 二进制字符的“大端模式”或“小端模式”
    hsize_t size = H5Tget_size(datatype);            // 数据类型的大小（字节数）。
    if (type_class == H5T_INTEGER)
        printf("dataset type is INTEGER\n");
    else if (type_class == H5T_FLOAT)
        printf("dataset type is FLOAT\n");
    if (data_order == H5T_ORDER_LE)
        printf("dataset is Little Endian Order\n");
    else
        printf("dataset is Big Endian Order\n");
    printf("dataset type size is %lld\n", size);

    // 提取数据集空间的相关信息。
    hid_t dataspace = H5Dget_space(dataset_id);           // dataspace handler, 数据集空间处理器。
    hsize_t rank = H5Sget_simple_extent_ndims(dataspace); // 数据集维度。
    hsize_t dims[2];
    H5Sget_simple_extent_dims(dataspace, dims, NULL); // 数据集每个维度的长度。
    printf("dataspace rank %lld, dimensions %lu * %lu\n", rank, (unsigned long)(dims[0]), (unsigned long)(dims[1]));

    // herr_t H5Dread(
    //     hid_t dset_id,       读取目标数据集id
    //     hid_t mem_type_id,   内存数据类型
    //     hid_t mem_space_id,  内存数据空间
    //     hid_t file_space_id, 文件数据空间
    //     hid_t plist_id,      IO操作类型
    //     void *buf)           待写入数据缓冲区
    status = H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, matrix[0]);

    print_matrix_2d(rows, columns, matrix);
    printf("dataset readed, status: %d\n", status);

    status = H5Dclose(dataset_id);
    status = H5Fclose(file_id);
    delete[] matrix;
}

void hdf5_subset_read()
{
    char *file_name = "file.h5";
    herr_t status;
    hid_t file_id = H5Fopen(file_name, H5F_ACC_RDONLY, H5P_DEFAULT);

    char *dataset_name = "MyGroup1/dset";
    hid_t dataset_id = H5Dopen(file_id, dataset_name, H5P_DEFAULT);

    int rank = 2;
    int dim0 = 10, dim1 = 8;
    int offset0 = 1, offset1 = 1;
    int sub_dim0 = 3, sub_dim1 = 4;

    // 定义子集(block, 块)的四大属性
    hsize_t count[2] = {sub_dim0, sub_dim1}; // 块大小
    hsize_t offset[2] = {offset0, offset1};  // 块起始位置
    hsize_t stride[2] = {1, 1};              // 块取值间隔
    hsize_t block[2] = {1, 1};               // 块数量
    hsize_t dims[2] = {dim0, dim1};          // 原始数据集大小

    // 定义块在文件数据集空间
    hid_t dataspace_id = H5Dget_space(dataset_id);
    status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, offset, stride, count, block);

    // 定义块在内存数据集空间
    hid_t memspace_id = H5Screate_simple(rank, count, NULL);
    hsize_t mem_offset[2] = {0, 0};
    // status = H5Sselect_hyperslab(memspace_id, H5S_SELECT_SET, mem_offset, stride, count, block);

    double **sub_matrix;
    generate_matrix(sub_dim0, sub_dim1, sub_matrix, 0);

    // 从文件中的超块读取数据到内存中的块。
    // status = H5Dread(dataset_id, H5T_NATIVE_DOUBLE, memspace_id, dataspace_id, H5P_DEFAULT, sub_matrix);
    // print_matrix_2d(sub_dim0, sub_dim1, sub_matrix);

    printf("!!! sub dataset reading failed.\n");
}

void hdf5_bocks_compress(char mode)
{
    char *file_name = "file.h5";
    hid_t file_id = H5Fcreate(file_name, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status;

    int rows = 1000, columns = 800;
    double **matrix;
    generate_matrix(rows, columns, matrix);

    unsigned int rank = 2;
    hsize_t dims[2];
    dims[0] = rows;
    dims[1] = columns;

    hid_t dataspace_id = H5Screate_simple(rank, dims, NULL);

    hid_t plist_d = H5Pcreate(H5P_DATASET_CREATE);
    hsize_t block_dims[2] = {200, 80};
    status = H5Pset_chunk(plist_d, rank, block_dims);

    if (mode == 'z')
    {
        status = H5Pset_deflate(plist_d, 6);
    }
    else if (mode == 's')
    {
        status = H5Pset_szip(plist_d, H5_SZIP_NN_OPTION_MASK, 32);
    }
    else
    {
        printf("compression mode must be 'z' or 's'.\n");
        status = H5Pclose(plist_d);
        status = H5Sclose(dataspace_id);
        status = H5Fclose(file_id);
        return;
    }

    hid_t dataset_id = H5Dcreate(file_id, "zipset", H5T_NATIVE_DOUBLE, dataspace_id, H5P_DEFAULT, plist_d, H5P_DEFAULT);

    status = H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, matrix);

    status = H5Dclose(dataset_id);
    status = H5Sclose(dataspace_id);
    status = H5Pclose(plist_d);
    status = H5Fclose(file_id);
    delete[] matrix;
}