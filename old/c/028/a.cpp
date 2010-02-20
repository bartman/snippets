#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <map>
#include <list>

template <class F>
class allocator {
        std::list<F*> m_list;
        typedef typename std::list<F*>::iterator list_iterator;
public:
        allocator () {}
        ~allocator () 
        {
                list_iterator i;
                while ( (i = m_list.begin()) != m_list.end() ) {
                        F* f = *i;
                        m_list.erase(i);
                        std::cout << "delete " << f << "\n";
                        delete f;
                }
        }

        F* create ()
        {
                F* f = new F();
		std::cout << "new " << f << "\n";
                m_list.push_back(f);
                return f;
        }

        void destroy (F *f)
        {
		std::cout << "delete " << f << "\n";
                list_iterator i = m_list.find (f);
                std::cout << " i = " << i << "\n";
                if (i != m_list.end()) {
                        m_list.erase(i);
                        delete f;
                }
        }

};



int
main (void)
{
        allocator<std::string> a;

        std::string *s = a.create();

        s->assign ("123");

        std::cout << "'" << *s << "'\n";



        return 0;
}
