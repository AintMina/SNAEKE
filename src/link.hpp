#pragma once



#include <vector>

class Neuron;


class Link {
private:
    int id;
    int in_id;
    int out_id;
    double weigth;
    bool enabled = 1;

public:
    double value = 0.0;
    bool calculated = false;
    
    Link(int id, int in, int out, double weigth);

    void enable();
    void disable();

    int get_id() const;
    void set_id(int id);

    double get_weigth();
    void set_weigth(double weight);
    int get_in();
    void set_in(int in);
    int get_out();
    void set_out(int out);

    void calculate_value(std::vector<Link>& links, std::vector<Neuron>& neurons);
    double get_value(std::vector<Link>& links, std::vector<Neuron>& neurons);

};