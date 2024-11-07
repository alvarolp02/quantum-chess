#include "quantum_chess/qc_tree.hpp"

QCTree::QCTree(Board B){
    this->root = new QCNode(B,0);
}

void QCTree::propagate(Tile source, Tile target){
    this->root->propagate(source, target);
}

void QCTree::split(Tile source, Tile target1, Tile target2){
    this->root->split(source, target1, target2);
}

void QCTree::print_tree(){
    print_tree_aux(this->root, "", false);
    
    std::cout  << std::endl << std::endl << std::endl;
}

void QCTree::print_tree_aux(QCNode* node, std::string prefix, bool split = false){
    if(node==nullptr){
        return;
    }

    if(split){
        std::cout << std::endl;
        std::cout << prefix;
        std::cout << "└─ ";
        prefix += "    ";
    } else {
        std::cout << "── "; 
        prefix += "    "; 
    }
    std::cout << node->index;

    print_tree_aux(node->next, prefix, false);
    print_tree_aux(node->split1, prefix, true);
    print_tree_aux(node->split2, prefix, true);
    
}


