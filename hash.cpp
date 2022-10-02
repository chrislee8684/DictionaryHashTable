#include "hash.h"


    hashTable::hashTable(int size):capacity(getPrime(size)),filled(0), data(capacity){}


    int hashTable::insert(const std::string &key) {
        unsigned int hash_value = hash(key);
        unsigned int position = findPos(key);

        if (filled>=capacity*0.6) {// always see if rehash needs to be done first, so don't have to deal with overfilling problem later
            bool rehashed = rehash();

            if (!rehashed){
                return 2;
            }
        }

        if(position==-1){ //key not in ht yet; first cell it looks at is empty
            data[hash_value].key = key;
            data[hash_value].isOccupied = true;
            filled++;

            return 0;
        }
        else{ //either duplicate or linear probed
            if (data[position].key == key){ //duplciate
                return 1;
            }
            else{ //linear probed
                data[position].key = key;
                data[position].isOccupied = true;
                filled++;

                return 0;
            }
        }
    }

    bool hashTable::contains(const std::string &key) { //looking for duplicates
    if (findPos(key) == -1 || data[findPos(key)].key!=key) { //first cell it looks at is empty or linear probing didn't work
            return false;
        }
    else { //duplicate
            return true;
        }
    }




    int hashTable::hash(const std::string &key) {
        unsigned int hash_value = 0;

        for (char ch : key){
            hash_value = 37 * hash_value + ch; //Ref: Horner's rule from figure 5.4 of Weiss
        }
        return hash_value % capacity;
    }

    int hashTable::findPos(const std::string &key){ //sees if value exists at index
        unsigned int value = hash(key);

        if(!data[value].isOccupied){ //index not filled
            return -1;
        }
        else{ //index already filled
            while (data[value].key!=key){
                value = (value+1) % capacity;

                if(!data[value].isOccupied){ //index not filled
                    return value;
                }
            }
        }
        return value; //return value if duplicate
    }

    bool hashTable::rehash(){
        unsigned int new_size_min = filled*3; //filled reduced from 60% to 33%
        unsigned int new_prime = getPrime(new_size_min);

        std::vector<hashItem> copy = data; //copy old ht

        data.clear(); //clear old data
        filled=0; //update filled & capacity
        capacity=new_prime;
        data.resize(capacity);

        for (hashItem item: copy){
            if (item.isOccupied){ //if old data contains value, reinsert into new
                insert(item.key);
            }
        }

        if (filled < (capacity * 0.6)){ //rehash success

            return true;
        }
       else
       {
           return false;
       }
    }


    unsigned int hashTable::getPrime(int size){


        unsigned int prime_sequence[] = {1949, 7877, 11551, 52639, 220579, 500069, 700319, 800647, 1000099, 3000073, 6000641, 9001481};

        for (int i=0; i<sizeof(prime_sequence); i++) {
            while (size < prime_sequence[i]) {
                return prime_sequence[i];
            }
        }
        return 0; //all available hash values too small

};
