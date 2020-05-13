#include "Config.h"
#include "NodeGene.h"

#include <bits/stdc++.h>

int main() {
    NodeGene n1(5, HIDDEN);
    NodeGene n2(n1);
    std::cout<<n1.get_id()<<std::endl;
    std::cout<<n2.get_type()<<std::endl;
}