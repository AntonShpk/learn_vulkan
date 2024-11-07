#pragma once

#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>

namespace Tools {

    class Random {
    public:
        Random();
        ~Random() = default;
        template<class T>
        void Fill(std::vector<T> &vec, std::function<T(void)> gen);

        std::mt19937 &GetEngine() { return mersenne_engine; }

    private:
        //instance of an engine.
        std::random_device rnd_device;
        std::mt19937 mersenne_engine;
    };


    template<class T>
    void Random::Fill(std::vector<T> &vec, std::function<T()> gen) {
        std::uniform_real_distribution<float> dist{-1., 1.};
        std::generate(vec.begin(), vec.end(), gen);
    }

}