/*
 * @project string from bastion project
 * @file string.hpp
 * @author Soundwave
 * @date unkown
 * @brief:
 * 	char array container
 * 		Wrapper class of c-style chars array
 * 		
 * 	Future plans:
 * 	Small-string optimization
 * */
#ifndef string_h
#define string_h
#include <memory> //for std::unique_ptr 
#include <string.h> // for strlen
#include <ostream> //for operator<< 
#include <istream> // for operator>>
namespace bastion{
    class string{
    private:
        //Smart pointer on raw data(char[])
        std::unique_ptr<char[]> data;
        //Amount of char in string
        unsigned int size_{0};
    public:
        //=============================================
        //             Initialization
        //=============================================
        /* default:
        *   string(void)
        */
        string() = default;
        /* Values constructor:
        *   copy array of chars to heap
        */
        string(const char text[]):
            data(std::unique_ptr<char[]>(new char[strlen(text)])),
            size_(strlen(text))
        {
            std::copy(text,text+this->size_,this->data.get());
        }

        /* Copy and Move constructors 
        *   string(const array&)
        *   string(array&&)
        */
        string(const string& source){
            this->size_ = source.size_;
            this->data = std::unique_ptr<char[]>(new char[source.size_]);
            std::copy(source.data.get(), source.data.get()+source.size_,this->data.get());
        }

        string(string&&source):
            data(std::move(source.data)),
            size_(std::move(source.size_))
        {}
        /* Initialization operators
        *   operator=(const string&)
        *   operator=(string&&)
        */
        string& operator= (const string& source){
            /*copy&swap idiom*/
            string timed{source};
            this->swap(timed);
            return *this;
        }

        string& operator= ( string&&source){
            //in theory variant with this == &source incredible, because rvalue can't be "this object"
            /*if(this == &source){
                return *this;
            }*/
            this->size_ = source.size_;
            this->data = std::move(source.data);
            return *this;
        }
        //=============================================
        //              Stream Operators
        //=============================================
        /*
        *   std::ostream&operator<<(std::ostream&,const string&)
        *   std::istream&operator>>(std::istream&stream,string&)
        */
        friend std::ostream& operator<<(std::ostream& stream, const string& source/*можно просто string source, но так вызывается конструктор копирования*/){
            stream<< source.data.get();
            return stream;
        }
        friend std::istream& operator>>(std::istream& stream, string& source){
            int num = stream.gcount();
            std::unique_ptr<char[]> simbols = std::unique_ptr<char[]>(new char[num]);
            stream>> simbols.get();
            source.size_ = strlen(simbols.get());
            std::copy(simbols.get(), simbols.get()+source.size_, source.data.get());
            return stream;
        }
        //=============================================
        //              Data Access
        //=============================================
        /*
        *   operator[] -- get link to raw data(char) element,without memery check
        *   at -- get link to raw data element, with memory check
        */
        char& operator[](const int n){
            return data.get()[n];
        }
        char& at(const uint n){
            if(n<this->size_){
                return this->data.get()[n];
            }
        }
        /* Iterators
        *   access[begin,end) to pointers of string
        *   const access[begin,end) to pointers of string
        */
        char* begin(){return this->data.get();}
        char* end(){return this->data.get()+this->size_;}
        const char* const cbegin(){return this->data.get();}
        const char* const cend(){return this->data.get()+this->size_;}
        /* First and Last elements
        * access to first and last elements in the string
        * const access to first and last elements in the sting
        */
        char&front(){return *this->data.get();}
        char&back(){return *(this->data.get()+this->size_);}
        const char& cfront()const{return *this->data.get();}
        const char& cback()const{return *(this->data.get()+this->size_);}

        //=============================================
        //                Any Utilites
        //=============================================  
        /* 
        * Get size of string
        * alias length
        */
        constexpr int size(){
            return this->size_;
        }
        constexpr int length(){
            return this->size_;
        }
        /* Set text
        */
        void set_text(const char text[]){
            //this->data = std::make_unique<char[]>(strlen(text));
            this->data = std::unique_ptr<char[]>(new char[strlen(text)]);
            this->size_ = strlen(text);
            std::copy(text,text+strlen(text),this->data.get());
        }
        /* Swap 
        *   Swap pointers
        */
        void swap(string&source){
            std::swap(this->data,source.data);
        }
        /**
         * c_str()
        */
        char* c_str(){return this->data.get();}
        //=============================================
        //                Operators
        //=============================================  
        
        /* 
        *   operator+= 
        *   operator+
        *   deleted:
        *       operator-    
        */
        string& operator+=(const char text[]){
            //auto ptr = std::make_unique<char[]>(this->size_+strlen(text));
            auto ptr = std::unique_ptr<char[]>(new char[this->size_ + strlen(text)]);
            std::copy(this->data.get(), this->data.get()+this->size_, ptr.get());
            std::copy(text,text+strlen(text),ptr.get()+this->size_);
            this->data = std::move(ptr);
            this->size_ += strlen(text);
            return *this;
        }
        
        string& operator+=(const string& source){
            //auto ptr = std::make_unique<char[]>(this->size_+source.size_);
            auto ptr = std::unique_ptr<char[]>(new char[this->size_ + source.size_]);
            std::copy(this->data.get(),this->data.get()+this->size_,ptr.get());
            std::copy(source.data.get(),source.data.get()+source.size_,ptr.get()+this->size_);
            this->data = std::move(ptr);
            this->size_ +=source.size_;
            return *this;
        }

        string operator+(const string&source){
            bastion::string new_string{};
            //new_string.data = std::make_unique<char[]>(this->size_+source.size_);
            new_string.data = std::unique_ptr<char[]>(new char[this->size_ + source.size_]);
            std::copy(this->data.get(), this->data.get()+this->size_, new_string.data.get());
            std::copy(source.data.get(), source.data.get()+source.size_, new_string.data.get()+this->size_);
            new_string.size_ = this->size_ + source.size_;
            return new_string;
        }
        string operator+(const char text[]){
            bastion::string new_string{};
            //new_string.data = std::make_unique<char[]>(this->size_+strlen(text));
            new_string.data = std::unique_ptr<char[]>(new char[this->size_ + strlen(text)]);
            std::copy(this->data.get(), this->data.get()+this->size_, new_string.data.get());
            std::copy(text, text+strlen(text), new_string.data.get()+this->size_);       
            new_string.size_ = this->size_ + strlen(text);
            return new_string;
        }
        string&operator-(const string&) = delete;
        string&operator-(const char[]) = delete;
        
    };
}
#endif // !string_h
