/*
 * mat3.h
 *
 *  Created on: 24 Nov 2009
 *      Author: phg
 */

#ifndef MAT3_H_
#define MAT3_H_


#define DEBUG
#ifndef DBG_LVL
#define DBG_LVL (DBG_ERR)
#endif
#include "debug.h"

#include <iostream>
#include <itpp/itbase.h>
#include <vector>
#include <assert.h>

using std::vector;
using namespace itpp;

template <typename T> class Mat3
{
public:
        ~Mat3(){};
        Mat3(int size_x = 0, int size_y = 0, int size_z = 0);

        void zeros();
        void zeros(int sx, int sy, int sz);
        void ones();
        void ones(int sx, int sy, int sz);

        void set_mat_xy(int zind, const Mat<T>& m);
        void set_mat_xz(int yind, const Mat<T>& m);
        void set_mat_yz(int xind, const Mat<T>& m);

        void add_mat_xy(const Mat<T>& m);
        //void add_mat_xz(const Mat<T>& m);
        //void add_mat_zy(const Mat<T>& m);

        void set_vec_x(int yind, int zind, const Vec<T>& v);
        void set_vec_y(int xind, int zind, const Vec<T>& v);
        void set_vec_z(int xind, int yind, const Vec<T>& v);

        Mat<T> get_mat_xy(int zind) const;
        Mat<T> get_mat_yz(int xind) const;
        Mat<T> get_mat_xz(int yind) const;

        Mat3<T> get_submat(int xi0,int xi1, int yi0,int yi1 , int zi0,int zi1) const;
        //Mat<T> get_submat_xy(int zind);
        //Mat<T> get_submat_yz(int xind);
        //Mat<T> get_submat_xz(int yind);

        Vec<T> get_vec_x(int yind,int zind);
        Vec<T> get_vec_y(int xind,int zind);
        Vec<T> get_vec_z(int xind,int yind);

        int get_size_y() const {return size_y;}
        int get_size_x() const {return size_x;}
        int get_size_z() const {return size_z;}

        void print();

        T get_elem(int yi, int xi, int zi){return vData[zi].get(yi,xi);}


        //OPERATORS
        void operator=(const Mat3<T> &m);  //CONST CORRECTNESS MISSING!
        Mat3<T> operator+(const Mat3<T> &m);
        Mat3<T> operator+(const T &t);
        Mat3<T> operator-(const Mat3<T> &m);
        Mat3<T> operator-(const T &t);
        Mat3<T> operator*(const Mat3<T> &m);
        Mat3<T> operator*(const T &t);




protected:
        int size_x;//row
        int size_y;//col
        int size_z;
        vector< Mat<T> > vData;

        //shared_ptr<const T> pData;
};


typedef Mat3<double> mat3;
typedef Mat3<std::complex<double> > cmat3;
typedef Mat3<int> imat3;


template <typename T> Mat3<T>::Mat3(int size_x, int size_y, int size_z)
                                  : size_x(size_x), size_y(size_y), size_z(size_z)
{
        Mat<T> tmpMat(size_x, size_y);
        tmpMat.zeros();
        for (int i=0; i<size_z; i++)
        {
                vData.push_back(tmpMat);
        }
}


template <typename T> void Mat3<T>::zeros()
{
        for (int i=0; i<size_z; i++)
        {
                vData[i].zeros();
        }
}

template <typename T> void Mat3<T>::zeros(int sx, int sy, int sz)
{
        vData.clear();
        size_x=sx;
        size_y=sy;
        size_z=sz;
        for (int i=0; i<size_z; i++)
        {
                Mat<T> tmpMat(size_x, size_y);
                tmpMat.zeros();
                for (int i=0; i<size_z; i++)
                {
                        vData.push_back(tmpMat);
                }
        }
}


template <typename T> void Mat3<T>::ones()
{
        for (int i=0; i<size_z; i++)
        {
                vData[i].ones();
        }
}

template <typename T> void Mat3<T>::ones(int sx, int sy, int sz)
{
        vData.clear();
        size_x=sx;
        size_y=sy;
        size_z=sz;
        for (int i=0; i<size_z; i++)
        {
                Mat<T> tmpMat(size_x, size_y);
                tmpMat.ones();
                for (int i=0; i<size_z; i++)
                {
                        vData.push_back(tmpMat);
                }
        }
}

//####################################################
//################## SET_MAT_XX ######################
//####################################################
template <typename T> void Mat3<T>::set_mat_xy( int zind, const Mat<T>& m)
{
        if(m.cols()==size_y && m.rows()==size_x)
        {
                if(zind<size_z && zind>=0)
                {
                        vData[zind]=m;
                }
                else
                {
                        ERROR("Mat3<T>::set_mat_xy() - Index out of bounds!");
                        //std::cout<<"Error: Index out of bound! \n";
                        assert(0);
                }
        }
        else
        {
                ERROR("Mat3<T>::set_mat_xy() - Set matrix of wrong size!");
                //std::cout<<"Set matrix of wrong size! \n";
                assert(0);
        }
}

template <typename T> void Mat3<T>::set_mat_xz(int yind, const Mat<T>& m)
{
        //std::cout<<m.rows()<<" "<<size_x<<" "<<m.cols()<<" "<<size_z<<"\n";
        if(m.rows()==size_x && m.cols()==size_z)
        {
                if(yind<size_y && yind>=0)
                {
                        for (int i=0; i<size_z; i++)
                        {
                                vData[i].set_col(yind,m.get_col(i));
                        }
                }
                else
                {
                        ERROR("Mat3<T>::set_mat_xz() - Index out of bounds!");
                        //std::cout<<"Error: Index out of bound! \n";
                        assert(0);
                }
        }
        else
        {
                ERROR("Mat3<T>::set_mat_xz() - Set matrix of wrong size!");
                //std::cout<<"Set matrix of wrong size! \n";
                assert(0);
        }
}

template <typename T> void Mat3<T>::set_mat_yz(int xind, const Mat<T>& m)
{
        if(m.rows()==size_y && m.cols()==size_z)
        {
                if(xind<size_x && xind>=0)
                {
                        for (int i=0; i<size_z; i++)
                        {
                                vData[i].set_row(xind,m.get_col(i));
                        }
                }
                else
                {
                        ERROR("Mat3<T>::set_mat_yz() - Index out of bounds!");
                        //std::cout<<"Error: Index out of bound! \n";
                        assert(0);
                }
        }
        else
        {
                ERROR("Mat3<T>::set_mat_yz() - Set matrix of wrong size!");
                //std::cout<<"Set matrix of wrong size! \n";
                assert(0);
        }
}

//####################################################
//################## SET_VEC_X #######################
//####################################################

template <typename T> void Mat3<T>::set_vec_x(int yind, int zind, const Vec<T>& v)
{
        if(v.length()==size_x)
        {
                if(yind<size_y && yind>=0 && zind<size_z && zind>=0)
                {
                        vData[zind].set_col(yind,v);
                }
                else
                {
                        ERROR("Mat3<T>::set_vec_x() - Index out of bounds!");
                        //std::cout<<"Error: Index out of bound! \n";
                        assert(0);
                }
        }
        else
        {
                ERROR("Mat3<T>::set_vec_x() - Set vector of wrong size!");
                //std::cout<<"Set vector of wrong size! \n";
                assert(0);
        }
}

template <typename T> void Mat3<T>::set_vec_y(int xind, int zind, const Vec<T>& v)
{
        if(v.length()==size_y)
        {
                if(xind<size_x && xind>=0 && zind<size_z && zind>=0)
                {
                        vData[zind].set_row(xind,v);
                }
                else
                {
                        ERROR("Mat3<T>::set_vec_y() - Index out of bounds!");
                        //std::cout<<"Error: Index out of bound! \n";
                        assert(0);
                }
        }
        else
        {
                ERROR("Mat3<T>::set_vec_y() - Set vector of wrong size!");
                //std::cout<<"Set vector of wrong size! \n";
                assert(0);
        }
}

template <typename T> void Mat3<T>::set_vec_z(int xind, int yind, const Vec<T>& v)
{
        if(v.length()==size_z)
        {
                if(xind<size_x && xind>=0 && yind<size_y && yind>=0)
                {
                        for (int i=0; i<size_z; i++)
                        {
                                vData[i].set(xind,yind,v.get(i));
                        }
                }
                else
                {
                        ERROR("Mat3<T>::set_vec_z() - Index out of bounds!");
                        //std::cout<<"Error: Index out of bound! \n";
                        assert(0);
                }
        }
        else
        {
                ERROR("Mat3<T>::set_vec_z() - Set vector of wrong size!");
                //std::cout<<"Set vector of wrong size! \n";
                assert(0);
        }
}

//####################################################
//################## ADD_MAT_XX ######################
//####################################################
template <typename T> void Mat3<T>::add_mat_xy(const Mat<T>& m)
{
        if(m.cols()==size_y && m.rows()==size_x)
        {
                        vData.push_back(m);
                        size_z++;
        }
        else
        {
                ERROR("Mat3<T>::add_mat_xy() - Set matrix of wrong size!");
                //std::cout<<"Set matrix of wrong size! \n";
                assert(0);
        }
}

//####################################################
//################## ADD_VEC_XX ######################
//####################################################



//####################################################
//################## GET_MAT_XX ######################
//####################################################
template <typename T> Mat<T> Mat3<T>::get_mat_xy(int zind) const
{
        if(zind<size_z && zind>=0)
        {
                return vData[zind];
        }
        else
        {
                ERROR("Mat3<T>::get_mat_xy() - Index out of bounds!");
                //std::cout<<"Error: Index out of bound! \n";
                assert(0);
        }

}

template <typename T> Mat<T> Mat3<T>::get_mat_yz(int xind) const
{
        if(xind<size_x)
        {
                Mat<T> resultMat(size_z,size_y);
                for (int i=0; i<size_z; i++)
                {
                        resultMat.set_row(i,vData[i].get_row(xind));
                }
                return resultMat;
        }
        else
        {
                ERROR("Mat3<T>::get_mat_yz() - Index out of bounds!");
                //std::cout<<"Error: Index out of bound! \n";
                assert(0);
        }
}


template <typename T> Mat<T> Mat3<T>::get_mat_xz(int yind) const
{
        if(yind<size_y)
        {
                Mat<T> resultMat(size_x,size_z);
                for (int i=0; i<size_z; i++)
                {
                        resultMat.set_col(i,vData[i].get_col(yind));
                }
                return resultMat;
        }
        else
        {
                ERROR("Mat3<T>::get_mat_xz() - Index out of bounds!");
                //std::cout<<"Error: Index out of bound! \n";
                assert(0);
        }
}

//####################################################
//################## GET_SUBMAT_XX ###################
//####################################################
template <typename T> Mat3<T> Mat3<T>::get_submat(int xi0,int xi1, int yi0,int yi1 , int zi0, int zi1) const
{
        if(yi0 >=0 && yi1 >=0 && xi0 >=0 && xi1 >=0 && zi0 >=0 && zi1 >=0 &&
                        yi0 < size_y && yi1 < size_y && xi0 < size_x &&
                        xi1 < size_x && zi0 < size_z && zi1 < size_z )
        {
                int ylow = (yi0<yi1) ? yi0 : yi1;
                int yup = (yi0>=yi1) ? yi0 : yi1;
                int xlow = (xi0<xi1) ? xi0 : xi1;
                int xup = (xi0>=xi1) ? xi0 : xi1;
                int zlow = (zi0<zi1) ? zi0 : zi1;
                int zup = (zi0>=zi1) ? zi0 : zi1;

                Mat3<T> resultMat(xup-xlow+1,yup-ylow+1,zup-zlow+1);
                Mat<T> tmpMat;
                int k=0;
                for (int i=zlow ; i<=zup ; i++)
                {
                        tmpMat=vData[i].get(xlow,xup,ylow,yup);
                        resultMat.set_mat_xy(k,tmpMat);
                        k++;
                }
                return resultMat;
        }
        else
        {
                ERROR("Mat3<T>::get_submat() - Index out of bounds!");
                //std::cout<<"Error: Index out of bound! \n";
                assert(0);
        }
}



//####################################################
//################## GET_VEC_X #######################
//####################################################

template <typename T> Vec<T> Mat3<T>::get_vec_x(int yind, int zind)
{
        if(yind<size_y && yind>=0 && zind<size_z && zind>=0)
        {
                return vData[zind].get_col(yind);
        }
        else
        {
                ERROR("Mat3<T>::get_vec_x() - Index out of bounds!");
                //std::cout<<"Error: Index out of bound! \n";
                assert(0);
        }

}

template <typename T> Vec<T> Mat3<T>::get_vec_y(int xind, int zind)
{
        if(xind<size_x && xind>=0 && zind<size_z && zind>=0)
        {
                return vData[zind].get_row(xind);
        }
        else
        {
                ERROR("Mat3<T>::get_vec_y() - Index out of bounds!");
                //std::cout<<"Error: Index out of bound! \n";
                assert(0);
        }

}

template <typename T> Vec<T> Mat3<T>::get_vec_z(int xind, int yind)
{
        Vec<T> resultVec(size_z);
        if(xind<size_x && xind>=0 && yind<size_y && yind>=0)
        {
                for (int i=0; i<size_z; i++)
                {
                        resultVec(i)=vData[i].get(xind,yind);
                }
                return resultVec;
        }
        else
        {
                ERROR("Mat3<T>::get_vec_z() - Index out of bounds!");
                //std::cout<<"Error: Index out of bound! \n";
                assert(0);
        }

}

//####################################################
//############### OTHER FUNCTIONS ####################
//####################################################

template <typename T> void Mat3<T>::print()
{
        std::cout<<"{";
        for (int i=0; i<size_z; i++)
        {
                std::cout<<vData[i];
                if(i<size_z-1)
                {std::cout<<" ; \n";}

        }
        std::cout<<"}\n";
}

//=================================
//========== OPERATORS ============
//=================================
template <typename T> void Mat3<T>::operator=(const Mat3<T> &m)
{

        vData.clear();
        size_x=m.get_size_x();
        size_y=m.get_size_y();
        size_z=m.get_size_z();

        for (int i=0; i<size_z; i++)
        {
                vData.push_back(m.get_mat_xy(i));
        }
}

template <typename T> Mat3<T> Mat3<T>::operator+(const Mat3<T> &m)
{
        Mat3<T> resultMat(size_x, size_y, size_z);
        Mat<T> tmpMat;
        for (int i=0; i<size_z; i++)
        {
                resultMat.set_mat_xy(i,vData[i]+m.get_mat_xy(i));
        }
        return resultMat;
}

template <typename T> Mat3<T> Mat3<T>::operator+(const T &t)
{
        Mat3<T> resultMat(size_x, size_y, size_z);
        Mat<T> tmpMat;
        for (int i=0; i<size_z; i++)
        {
                resultMat.set_mat_xy(i,vData[i]+t);
        }
        return resultMat;
}

template <typename T> Mat3<T> Mat3<T>::operator-(const Mat3<T> &m)
{
        Mat3<T> resultMat(size_x, size_y, size_z);
        Mat<T> tmpMat;
        for (int i=0; i<size_z; i++)
        {
                resultMat.set_mat_xy(i,vData[i]-m.get_mat_xy(i));
        }
        return resultMat;
}

template <typename T> Mat3<T> Mat3<T>::operator-(const T &t)
{
        Mat3<T> resultMat(size_x, size_y, size_z);
        Mat<T> tmpMat;
        for (int i=0; i<size_z; i++)
        {
                resultMat.set_mat_xy(i,vData[i]-t);
        }
        return resultMat;
}

template <typename T> Mat3<T> Mat3<T>::operator*(const Mat3<T> &m)
{
        Mat3<T> resultMat(size_x, size_y, size_z);
        Mat<T> tmpMat;
        for (int i=0; i<size_z; i++)
        {
                resultMat.set_mat_xy(i,vData[i]*m.get_mat_xy(i));
        }
        return resultMat;
}

template <typename T> Mat3<T> Mat3<T>::operator*(const T &t)
{
        Mat3<T> resultMat(size_x, size_y, size_z);
        Mat<T> tmpMat;
        for (int i=0; i<size_z; i++)
        {
                resultMat.set_mat_xy(i,vData[i]*t);
        }
        return resultMat;
}


#endif /* MAT3_H_ */
