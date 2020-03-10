/*
 * @project array from bastion project
 * @file array.hpp
 * @author Soundwave
 * @date unkown
 * @brief:
 * 	static size array Container
 * 	wrapper class of c-style array
 * */
/*
* @tparam T -- elements type
* @tparam n -- size of array
*/
#ifndef array_h
#define array_h
#include <algorithm> //for std::copy and std::move
#ifdef n_use_expression_in_array
#include <cassert> //for assert()
#else
//#include <h/exception.hpp>
#endif
namespace bastion{
    template<typename T, unsigned int n>
    class array{
    private:
        //Raw data:
        T _data[n];
    public:
        //=============================================
        //             Initialization
        //=============================================
        /* default:
        *   array(void)
        *   ~array(void)
        */
        array(void) = default;
        ~array(void) = default;
        /* Values constructor:
        * move a list of data to raw data
        * @tparam Ts -- parametr pack to move
        */
        template<typename ... Ts>
        array(const Ts ... ts):
            _data{std::move(ts)...}
        {}
        /* Copy and Move constructors 
        *   array(const array&)
        *   array(array&&)
        */
        template<typename T_copy,uint N_copy>
        array(const array<T_copy,N_copy>& source){
            #ifdef n_use_expression_in_array
                static_assert(std::is_same_v<T_copy,T>,"type mismatch");
                static_assert(N_copy<=n, "size error");
                std::copy(source.cbegin(),source.cend(),this->_data);
            #else
                if constexpr(std::is_same_v<T_copy,T>){
                    if constexpr(N_copy <=n ){
                        std::copy(source.cbegin(),source.cend(),this->_data);                        
                    }
                    else{
                        //вызвать exception
                    }
                }
                else{
                    //вызвать exception
                }
            #endif
        }
        template<typename T_move,uint N_move>
        array(array<T_move,N_move>&&source){
            #ifdef n_use_expression_in_array
                static_assert(std::is_same_v<T_move,T>,"type mismatch");
                static_assert(N_copy<=n,"size error");
                std::move(source.begin(),source.end(),this->_data);
            #else
                if constexpr(std::is_same_v<T_move,T>){
                    if constexpr(N_move <=n ){
                        std::move(source.begin(),source.end(),this->_data);
                    }
                    else{
                        //вызвать exception
                    }
                }
                else{
                    //вызвать exception
                }

            #endif
        }
        /* Initialization operators
        *   operator=(const array&)
        *   operator=(array&&)
        */

        array&operator=(const array& source){
            if (this == &source){
                return *this;
            }
            std::copy(source.cbegin(),source.cend(),this->_data);
            return *this;
        }
        array&operator=(array&&source){
            if (this == &source){ 
                return *this;
            }
            std::move(source.begin(),source.end(),this->_data);return *this;
        }

        //=============================================
        //              Data Access
        //=============================================
        /*
        * operator[] -- get link to raw data element, without memory check
        * at -- get link to raw data element, with memory check
        */
        T& operator[](const unsigned int count){
            return this->_data[count];
        }
        #ifndef n_use_expression_in_array
        T& at(const uint count){
            if(count< n ){
                return this->_data[count];
            }
            else{
                //кинуть exception
            }
        }
        #else
        T& at(const uint count){
            assert(count<n);
            return this->_data[count];
        }
        #endif
        /* Iterators
        * access [begin,end) to pointers of array
        * const access [begin,end) to pointers of arary
        */
        T* begin(){return this->_data;}
        T* end(){return this->_data+ n;} 
        const T* cbegin() const {return this->_data;}
        const T* cend() const {return this->_data+n;}
        /*First and Last elements
        * access to first and last elements in the array
        * const access to first and last elements in the array
        */
        T&front(){return *this->_data;}
        T&back(){return *(this->_data+n-1);}
        const T&cfront() const{return *this->_data;}
        const T&cback() const{return *(this->_data+n-1);}

        //=============================================
        //                Any Utilites
        //=============================================  
        /*Get size of array
        */
        constexpr uint size(){return n;}
        /* Get DataSize in bytes 
        */
        constexpr uint DataSize(){return sizeof(T)*n;}
        /* Fill
        * assigns %value% to all elements of arary
        */
        void fill(const T& value){std::fill(this->begin(),this->end(), value);}
        /*Set
        * set one element with index
        * set elements to array
        *   from list of elements(now not work)
        *   from bastion::array
        */
        void set_element(int index, T value){this->_data[index] = value;}
        //template<typename ... T_set>
        /*void set(T_set ... t_set){
            set_element()
        }*/
        #ifndef n_use_expression_in_array
        template<typename T_set, uint n_set>
        void set(const array<T_set,n_set>& source){
            if constexpr (std::is_same_v<T_set,T>){
                if constexpr (n_set<=n){
                    /*const int* begin = source.cbegin();
                    const int*end = source.cend();
                    int*data = this->begin();
                    while(begin!=end){
                        data = (int*)begin;
                        begin++;
                        data++;
                    }*/
                    //memcpy(this->_data,source._data,source.DataSize());
                    std::copy(source.cbegin(),source.cend(),this->_data);
                }
                else{
                    //вызвать exception
                }
            }
            else{
                //вызвать exception
            }
        }
        template<typename T_set,uint n_set>
        void set(array<T_set,n_set>&& source){
            if constexpr (std::is_same_v<T_set,T>){
                if constexpr (n_set<=n){
                    std::move(source.begin(),source.end(),this->_data);
                }
                else{
                    //вызвать exception
                }
            }
            else{
                //вызвать exception
            }
        }
        #else
        template<typename T_set, uint n_set>
        void set(const array<T_set,n_set>& source){
            static_assert(n_set<=n,"error with size");
            static_assert(std::is_same_v<T_set,T>,"error with types");
            std::copy(source.cbegin(),source.cend(),this->_data);
        }
        template<typename T_set,uint n_set>
        void set(array<T_set,n_set>&&source){
            static_assert(n_set<=n,"error with size");
            static_assert(std::is_same_v<T_set,T>,"error with types");
            std::move(source.begin(),source.end(),this->_data);
        }
        #endif
        /* Array copy
        *   copy data from rhs array 
        */
        #ifndef n_use_expression_in_array
        template<typename T_copy, uint n_copy>
        void copy(const array<T_copy,n_copy>&source){
            if constexpr (std::is_same_v<T_copy,T>){
                if constexpr (n_copy<=n){
                    std::copy(source.cbegin(),source.cend(),this->_data);
                }
                else{
                    //вызывать exception
                }
            }
            else{
                //вызвать exception
            }
        }
        template<typename T_copy, uint n_copy>
        void copy(array<T_copy,n_copy>&&source){
            if constexpr (std::is_same_v<T_copy,T>){
                if constexpr (n_copy<=n){
                    std::move(source.begin(),source.end(),this->_data);
                }
                else{
                    //вызывать exception
                }
            }
            else{
                //вызвать exception
            }
        }
        #else
        template<typename T_copy,uint n_copy>
        void copy(const array<T_copy,n_copy>&source){
            static_assert(std::is_same_v<T_copy,T>,"error with types");
            static_assert(n_copy<=n,"error with sizes");
            std::copy(source.cbegin(),source.cend(),this->_data);
        }
        template<typename T_copy,uint n_copy>
        void copy(array<T_copy,n_copy>&&source){
            static_assert(std::is_same_v<T_copy,T>,"error with types");
            static_assert(n_copy<=n,"error with sizes");
            std::move(source.begin(),source.end(),this->_data);
        }
        #endif
        /* Array move
        * move data from rhs array
        * now not working
        */
        /*void move(array&&source){
            std::move(source.begin(),source.end(),this->_data);
        }*/
        /* Array swap
        * swap data between lhs array and rhs array
        */
        #ifndef n_use_expression_in_array
        template<typename T_swap, uint n_swap>
        void swap(array<T_swap,n_swap>&source){
            //будет улучшена с помощью std::swap
            if constexpr(std::is_same_v<T_swap,T>){
                if constexpr(n == n_swap){
                    auto timed_array = std::move(source);
                    source = std::move(*this);
                    *this = std::move(timed_array);
                }
                else{
                    //вызвать exception
                }
            }
            else{
                //вызвать exception
            }
        }
        #else
        template<typename T_swap, uint n_swap>
        void swap(array<T_swap,n_swap>&source){
            static_assert(std::is_same_v<T_swap,T>,"error with types");
            static_assert(n_swap==n,"error with sizes");
            auto timed_array = std::move(source);
            source = std::move(*this);
            *this = std::move(timed_array);
        }
        #endif
    };
    /*Auto-dedection guide:
    */
    template<typename  T_array,typename ... T_data>
    array(T_array,T_data... ) -> array<T_array,1+sizeof...(T_data)>;
    /**
     * auto-dedection guide,like bastion::array{"hello"} --> array<char,5>{'h','e','l','l','o'}
    */
    /*template<typename T_array,uint N> 
    array(const T_array(&)[N]) -> array<T_array,N>;*/
}
#endif // !array_h
