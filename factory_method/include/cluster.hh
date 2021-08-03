#include "constituent.hh"

#ifndef CLUSTER_H
#define CLUSTER_H

namespace simulation{

template <typename type>
class cluster: public constituent<type>{

    public:

        void move(type *delta_x);

        type  element_pos(const int i, const int axis) const;
        type& element_pos(const int i, const int axis);

        void add_constituent(constituent<type> *single_element);

        void add_constituent_to_cell();
        void remove_constituent_from_cell();

    private:

        std::vector<constituent<type>*> elements;


};


}

#endif