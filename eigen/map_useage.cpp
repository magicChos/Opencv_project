#include <eigen3/Eigen/Core>
#include <iostream>

void test(){
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
    std::cerr << __FUNCTION__ << std::endl;
}

int main()
{
    int array[8];
    for(int i = 0; i < 8 ; ++i){
        array[i] = i;
    }

    std::cout << "column-major: " << Eigen::Map<Eigen::Matrix<int , 2 , 4> >(array) << std::endl;
    std::cout << "row-major: " << Eigen::Map<Eigen::Matrix<int , 2 , 4> >(array) << std::endl;
    std::cout << "Row-major using stride:\n" << Eigen::Map<Eigen::Matrix<int , 2 , 4> , Eigen::Unaligned , Eigen::Stride<1 , 4> >(array) << std::endl;
    
    double *update = new double[3];
    for (int i = 0; i < 3; ++i){
        update[i] = i ;
    }

    Eigen::Vector3d d = Eigen::Vector3d(update);
    std::cout << "d: \n" << std::endl;
    std::cout << d << std::endl;

    double data[9];
    for (int i = 0 ; i < 9 ; ++i)
    {
        data[i] = i ;
    }

    Eigen::Matrix<double, 3 , 3 , Eigen::RowMajor> matrix33 = Eigen::Map<Eigen::Matrix<double , 3 , 3> > (data) ;
    std::cout << "matrix33 = \n" ;
    std::cout << matrix33 << std::endl;

    std::cout << Eigen::Matrix<double,1,1>::Identity()*1/(1.0*1.0) << std::endl;

    test();
    return 0;
}