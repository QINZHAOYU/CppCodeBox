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
 *   see : https://blog.csdn.net/david830_wu/article/details/63782190
 *         https://zhuanlan.zhihu.com/p/104145585
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

void generate_matrix(int rows, int columns, double *data_mem, double **matrix)
{
    data_mem = new double[rows * columns];
    matrix = new double *[rows];

    for (int i = 0; i < rows; ++i)
    {
        // 为矩阵分配内存，这里通过一维数组分配连续内存提高效率。
        matrix[i] = data_mem + i * columns;
    }

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < colums; ++c)
        {
            matrix[r][c] = r + 0.01 * c;
        }
    }
}

void hdf5_dataset_create_write()
{
    char *file_name = "file.h5";
    hid_t file_id = H5Fopen(file_name, H5F_ACC_RDWR, H5P_DEFAULT);
    herr_t status;

    /*dataspace一共有三种形式:
        + H5S_SCALAR : 标量，只有一个元素，rank为0;
        + H5S_SIMPLE：正常的数组形式的元素;
        + H5S_NULL：没有数据元素;
    */

    int rows = 10, columns = 8;
    double *data_mem;
    double **matrix;
    generate_matrix(rows, columns, data_mem, matrix);

    unsigned int rank = 2;
    hsize_t dims[2];
    dims[0] = rows;
    dims[1] = columns;

    hid_t dataspace_id = H5Screate_simple(rank, dims, NULL);
    hid_t dataset_id = H5Dcreate(file_id, "/dset", H5T_NATIVE_DOUBLE, dataspace_id);
    status = H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5A_ALL, H5P_DEFAULT, matrix[0]);

    status = H5Dclose(dataset_id);
    status = H5Dclose(dataspace_id);

    status = H5Fclose(file_id);
    delete[] matrix;
    delete[] data_mem;
}