#include <iostream>
#include <vector>
#include <map>

template <class T, bool use_size = false, size_t __s = 0 >
struct PullAllocator
{
    friend struct PullAllocator;
private:
    T* pull;
    int* numbers_of_vladel;
    int index;
    size_t s;
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;




    PullAllocator(size_t _s = 512) noexcept : numbers_of_vladel(0), index(0), s(use_size ? __s : _s) {
        try
        {
            pull = static_cast<T*>(std::malloc(s * sizeof(T)));
        }
        catch (...)
        {
            pull = static_cast<T*>(std::malloc(sizeof(T)));
        }
    }
    template <class U>
    PullAllocator(PullAllocator<U>&& a) noexcept : numbers_of_vladel(a.numbers_of_vladel), index(a.index), s(a.s), pull((T*)a.pull)
    {
        a.numbers_of_vladel++;
    }


    T* allocate(size_t n)
    {

        T* ptr = &pull[index];
        index += n;
        if ((size_t)index > s)
        {
            delete ptr;
            throw std::bad_alloc();
        }
        return ptr;

    }
    void deallocate(T* p, size_t n) {};

    template <class Up, class... Args>
    void construct(Up* p, Args &&...args)
    {
        ::new
        (p)
            Up(std::forward<Args>(args)...);

    }

    template<typename T2>
    void destroy(T2* p)
    {
        p->~T2();
    }

    ~PullAllocator() noexcept
    {
        numbers_of_vladel--;

        if (numbers_of_vladel == 0)
        {
            delete numbers_of_vladel;
            std::free(static_cast<void*>(pull));
        }
    }


    template <class U>
    struct rebind
    {
        typedef PullAllocator<U, use_size, __s> other;
    };

    template<typename U>
    bool operator==(PullAllocator<U> al)
    {
        return (al.pull == pull);
    }

};


namespace MyList
{
    template<typename T>
    struct Node
    {
        Node* next_node;
        T element;
    };

    template<typename T, typename Allocator>
    struct Iterator
    {
        Node<T>* my_node;
        Node<T> help_node;

        Allocator al;

        Iterator& operator++() noexcept
        {
            return pryam_incr();
        }

        Iterator operator++(int) noexcept
        {
            help_node.element = my_node->element;
            help_node.next_node = my_node;
            my_node = my_node->next_node;
            return help_node;
        }

        Iterator* pryam_incr() noexcept
        {
            my_node = my_node->next_node;
            return this;
        }


        operator T* ()
        {
            T* element = al.allocate(1);
            al.construct(element);
            element = &(my_node->element);


            return element;
        }
    };

    template <typename T, typename Allocator = std::allocator<T>>
    struct Conteiner
    {
    private:
        Allocator al;
        Node<T>* first_node;
        Node<T>* last_node;

        Iterator<T, Allocator>* get_iterator_from_node(Node<T>* nd)
        {
            Iterator < T, Allocator >* it = iter_al.allocate(1);
            iter_al.construct(it);

            it->my_node = nd;
            return it;
        }

        using Node_allocator = typename Allocator:: template rebind< Node<T>>:: other;
        using Iterator_allocator = typename Allocator:: template  rebind<Iterator<T, Allocator>>:: other;


        Node_allocator node_al;
        Iterator_allocator iter_al;
    public:



        Conteiner()
        {

            Node<T>* nds = node_al.allocate(2);
            node_al.construct(nds);
            node_al.construct(nds + 1);

            first_node = nds;
            last_node = nds + 1;
            first_node->next_node = last_node;
            last_node->next_node = nullptr;
        }

        void push_back(T element)
        {




            Node<T>* nds = node_al.allocate(1);
            node_al.construct(nds);

            last_node->element = element;
            last_node->next_node = nds;
            last_node = last_node->next_node;
        }

        Iterator<T, Allocator>* begin()
        {
            return get_iterator_from_node(first_node)->pryam_incr();
        }


        Iterator<T, Allocator>* end()
        {
            return get_iterator_from_node(last_node);
        }

    };

};


int main()
{
    std::map<int, int> mp;

    int factorial = 1;

    mp.insert(std::pair<int, int>(0, factorial));

    for (int i = 1; i < 10; i++)
    {
        factorial *= i;
        mp.insert(std::pair<int, int>(i, factorial));
    }

    std::map<int, int, PullAllocator<std::pair<int, int>, true, 12>> _mp;

    factorial = 1;

    mp.insert(std::pair<int, int>(0, factorial));

    for (int i = 1; i < 10; i++)
    {
        factorial *= i;
        mp.insert(std::pair<int, int>(i, factorial));
    }

    for (int i = 0; i < 10; i++)
    {
        std::cout << i << " " << mp[i] << std::endl;
    }

    MyList::Conteiner<int> ml;

    for (int i = 0; i < 10; i++)
    {
        ml.push_back(i);
    }

    MyList::Conteiner<int, PullAllocator<int, true, 12>> _ml;

    for (int i = 0; i < 10; i++)
    {
        _ml.push_back(i);
    }

    for (auto k = _ml.begin(); k->my_node->next_node != nullptr; k->pryam_incr())
    {
        std::cout << k->my_node->element << " ";
    }
}
