#pragma once

#include <vector>
#include "Oferta.h"
#include <algorithm> 
#include <random> 
using std::vector;
using std::shuffle;
#include <random>
#include <fstream>
#include <algorithm>

/*
Clasa de exceptii specifice Wishlist
*/
class WishlistException {
private:
    string errorMsg;
public:
    WishlistException(string errorMsg) :errorMsg{ errorMsg } {};
    string getErrorMessage() {
        return this->errorMsg;
    }
};

class WishlistRepo {
private:
    vector<Oferta> Wishlist;
public:
    void empty_wishlist();
    void add_wishlist(const Oferta& of);
    void generate_wishlist(int nr_oferte, const vector<Oferta>& memory);
    void export_wishlist(const string& name_file);
    const vector<Oferta>& get_Wishlist();
    int search_oferta_wishlist(const string& denumire, const string& destinatie);

};

void testWishlist();
