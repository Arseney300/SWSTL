/**
 * @project vector from bastion project
 * @file vector.hpp
 * @author Soundwave
 * @date 13.09.2020
 * @brief:
 *  dynamic size array container
 *  wrapper class of c-style dynamic class
 *  two types of vector:
 *   -dynamic vector with 2x multiplier
 *   -dynamic vector with golden_ratio(~1.6) multiplier
*/
/**
 * Future plans:
 * 1) create or init objects from std::init_list(or other, idk, i need to see cppcon: https://www.youtube.com/watch?v=tL44rnyLQ9Y)
 *          or use auto-dedection guide(idk how use it in vector, it doesn't even work in bastion::array)
 * 2) add exception
 *    and add static_assert (if user don't want use exception(is it possible???), anyway i can do it and user can set it with #ifned n_use_expression_in_vector)
 * 3) fix golder_ratio_calc (problem descibe in function comment)
 * 4) mb delete std::unique_ptr and use raw pointer, because unique_ptr isn't zero value construction (and create destructor of course)
*/
/**
 * @tparam T -- elments type
*/
#ifndef vector_h
#define vector_h
#include <memory> //for std::copy,std::move, std::unique_ptr
#include <math.h> //for sqrt(), round()
#define BASTION_VECTOR_MULTIPLIER 2
namespace bastion{
    /**
     * Golden ratio calc 
     * ?????? about performane (gdb show, that constexpr doesn't work, because i run sqrt(), which can't be constexpr). 
     * Solution: use calculated number in this function
    */
    constexpr float golden_ratio_calc(){
        return (1+sqrt(5))/2;
    }
    namespace __vector{
        /**
         * ABOUT VECTOR SIZE CALCULATING:
         * Standard CPP foundation does'n say about multiplier. They left it at the mercy of compiler developers
         * For example: gcc use 2x vector multiplier
         *              clang use 2x too
         *              Microsoft' cl use strange multipiplier: or it's dynamic or equial ~1.5
         * In theory one of the best multiplier is golden ratio  (https://en.wikipedia.org/wiki/Golden_ratio ) ~1.618
         * 
         * bastion::vector use 2x multiplier by default (you can change it with special define: BASTION_VECTOR_MULTI PLIER !!before include vector.hpp)
         * bastion:gr_vector use golder ration multipiplier
        */
        constexpr unsigned int vector_multiplier{BASTION_VECTOR_MULTIPLIER}; // like in gcc
    

        /**
         * vector_size struct
         * two members:
         *  size -- show current number of elements in vector
         *  capacity -- show how many elemets can be add
        */
        struct vector_size{
        public:
            uint size{0}, capacity{0};
            vector_size(uint _size, uint _capacity):
                size(_size), capacity(_capacity)
            {}
        };
        /**
         * BASE VECTOR CLASS
        */
        template<typename T>
        class base_vector{
        private:
            typedef unsigned int arg_type; 
        protected:
            /**
             * smart pointer on data
             * as unique_ptr aren't zero value, i think be beter if i use raw pointer and just delete it in destructor
            */
            std::unique_ptr<T[]> data;
            //size member
            vector_size size_;
        public:
            //=============================================
            //              Initialization
            //=============================================
            /**
             * base_vector(arg_type,arg_type)
            */
            base_vector ( arg_type size = 0, arg_type capacity = 0):
                size_(size,capacity)
            {
                if(size_.size || size_.capacity)
                    this->data = std::unique_ptr<T[]>(new T[this->size_.size + this->size_.capacity]);
            }
            /**
             * Copy and Move constructors
             *  base_vector(const base_vector&)
             *  base_vector(base_vector&&)
            */
            base_vector( const base_vector&source):
                size_(source.size_.size, source.size_.capacity)
            {
                this->data = std::unique_ptr<T[]>(new T[this->size_.size + this->size_.capacity]);
                std::copy(source.cbegin(), source.cend(), this->data.get());
            }
            base_vector(base_vector&&source):
                size_(source.size_.size, source.size_.capacity)
            {
                this->data = std::move(source->data);
            }

            /**
             * Initialization operators
             * operator=(const base_vector&)
             * operator=(base_vector&&)
            */
            base_vector& operator=(const base_vector&source){
                //beatiful self-assigment check:
                if(this == &source){
                    return *this;
                }
                //analog with copy semantic:
                this->size_.size = source.size_.size;
                this->size_.capacity = source.size_.capacity;
                this->data = std::unique_ptr<T[]>(new T[source.size_.size + source.size_.capacity]);
                std::copy(source.cbegin(),source.cend(),this->data.get());
                return *this;
            }
            base_vector&operator=(base_vector&&source){
                //analog with move semantic:
                this->size_.size = source.size_.size;
                this->size_.capacity = source.size_.capacity;
                this->data = std::move(source.data);
                return *this;
            }
            //=============================================
            //              Initialization
            //=============================================
            /**
             * operator[] -- get link to raw data element, without limits check
             * at() -- get link to raw data element, with limits check
            */
            T&operator[](arg_type index)const{
                return this->data.get()[index];
            }
            T& at(arg_type index)const{
                //.... do something with exceptions ....
                if (index < this->size_.size){
                    //....
                }
                return this->data.get()[index];
            }
            /**
             * Iterators
             * access [begin,end) to pointers of vector
             * const access [begin,end) to pointers of vector
            */
            T*begin()const{return this->data.get();}
            T*end()const{return this->data.get() + this->size_.size;}
            const T*cbegin()const{return this->data.get();}
            const T*cend()const{return this->data.get() + this->size_.size;}
            /**
             * First and last element
             * access to first and last element in vector(link)
             * const access to first and last element in vector(link)
            */
            T&front()const{return *this->data.get();}
            T&back()const{return *(this->data.get() + this->size_.size -1);}
            const T&cfront()const{return *this->data.get();}
            const T&cback()const{return *(this->data.get() + this->size_.size -1);}
            
            //=============================================
            //                Any Utilites
            //============================================= 

            /**
             * Get size and capacity of vector
            */
            constexpr auto size() const{return this->size_.size;}
            constexpr auto capacity() const{return this->size_.capacity;}
            /**
             * Fill
             * assigns %value% to all elements of array
            */
            template<typename T_fill>
            void fill(const T_fill element){
                //... do something with expressions:
                if constexpr(std::is_same_v<T,T_fill>){
                    std::fill(this->data.get(), this->data.get()+ this->size_.size);
                }
                else{
                    //exception ...
                }
            }
            /**
             * Get DataSize in bytes
            */
            constexpr auto DataSize() const{return sizeof(T) * (this->size_.size + this->size_.capacity);}
        };

    } // !namespace __vector 

    /**
     * VECTOR CLASS
     *  use __vector::vector_multiplier
     *  
    */
    template<typename T>
    class vector : public __vector::base_vector<T>{
    public:
        //=============================================
        //             Initialization
        //=============================================
        vector(): __vector::base_vector<T>{0,0}
        {}
        vector(uint size): __vector::base_vector<T>{size,0}
        {}
        vector(uint size, T init): __vector::base_vector<T>{size,0}
        {
            this->fill(init);
        }
        /**
         * PUSH_BACK function
        */
        void push_back(T&element){
            if(this->size_.capacity > 0){
                this->data.get()[this->size_.size] = element;
                this->size_.size++;
                this->size_.capacity--;
            }
            else{
                //reallocating memory
                //2x rule
                const uint last_size = this->size_.size + this->size_.capacity;
                uint new_size = last_size*__vector::vector_multiplier;
                if(new_size == 0)
                    new_size = 1;
                auto timed_pointer = std::unique_ptr<T[]>(new T[new_size]);
                std::copy(this->data.get(), this->data.get()+ this->size_.size, timed_pointer.get());
                this->data = std::move(timed_pointer);
                this->size_.capacity = new_size - this->size_.size;
                this->data.get()[this->size_.size] = element;
                this->size_.size++;
                this->size_.capacity--;
            }
        }
    };
    /**
     * GR_VECTOR CLASS
     * use golder ratio
    */
    template<typename T>
    class gr_vector: public __vector::base_vector<T>{
    public:
        //=============================================
        //             Initialization
        //=============================================
        gr_vector(): __vector::base_vector<T>{0,0}
        {}
        gr_vector(uint size): __vector::base_vector<T>{size,0}
        {}
        gr_vector(uint size, T init): __vector::base_vector<T>{size,0}
        {
            this->fill(init);
        }
        /**
         * PUSH_BACK FUNCTION
        */
        void push_back(T& element){
            if(this->size_.capacity > 0){
                this->data.get()[this->size_.size] = element;
                this->size_.size++;
                this->size_.capacity--;
            }
            else{
                //reallocating memory
                //golden_ratio rule
                const uint last_size = this->size_.size + this->size_.capacity;
                uint new_size = round((double)last_size*bastion::golden_ratio_calc());
                if(new_size == 0)
                    new_size = 1;
                auto timed_pointer = std::unique_ptr<T[]>(new T[new_size]);
                std::copy(this->data.get(), this->data.get()+ this->size_.size, timed_pointer.get());
                this->data = std::move(timed_pointer);
                this->size_.capacity = new_size - this->size_.size;
                this->data.get()[this->size_.size] = element;
                this->size_.size++;
                this->size_.capacity--;
            }
        }

    };



}


#endif // !vector_h