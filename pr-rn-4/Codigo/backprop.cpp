// KEYWORDS: BACKPROPAGATION NEURAL NETWORK C++ SOURCE CODE

// this program implements and tests a neural network based on backpropagation.
// the network has one input layer, one hidden layer, and one output layer.
// the output layer and hidden layer contain complete neurons, each neuron having
// a vector of synaptic weights, whereas the input layer is just a vector of
// input values.

// in the comments below, "FNN book" refers to "Fundamentals of Neural Networks",
// by Laurene Fausett (ISBN 0-13-334186-0), which contains the algorithm used here.

// this code may be freely copied.
// programmer: mathieu@textelectric.net

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// a few global constants...
const double double_rand_max = (double)RAND_MAX;
const size_t bits_per_char = 8;
const double alpha = 0.2;  // learning rate
const double bias_value = 1.0;

// note: in the method that loads the training data, the strings will be
// resized to ensure that the number of bits in a string matches the number
// of nodes in the input layer (for the input strings) or the output layer
// (for the target strings), not counting the bias unit in the input layer.

// number of neurons in output layer (does not need to to match other layer sizes)
const size_t output_layer_size = 120;
// number of actual neurons in hidden layer (not counting the bias unit)
// this does not have to be the same as the input or output layer sizes.
const size_t hidden_layer_size = 80;
// number of data input nodes (not counting the bias unit)
const size_t input_layer_size = 120;

class target;
class node;

// abstract base class for the input layer, hidden layer, and output layer
class layer {
public:
    virtual double operator[] (size_t i) = 0;
    virtual size_t get_neuron_size() = 0;
    virtual size_t size() const = 0;
    virtual void adjust_weights() {
        throw "called method in abstract base class LAYER\n";
        }
    virtual void set_corrections(layer& input, target& training_value) {}
    virtual void set_corrections(layer& input) {
        throw "called method in abstract base class LAYER\n";
        }
    virtual node* get_node_ptr (size_t i) = 0;
};

class output_layer;

class node {
    // a node is either a neuron or a bias unit.  the point of declaring this
    // base class is to keep the bias unit and the neurons in a single vector for
    // the input layer and the hidden layer.  most data members of the "node" class are
    // unused in the case of the bias units, but there are only two bias units for the entire
    // network, and the vector objects are very small (24 bytes) when they are empty.
protected:
    vector<double> m_synapses;
    // cache these values to avoid recomputing them during the backpropagation phase
    double m_output;
    double m_error;   // "error information term" as described in the FNN book
    vector<double> m_correction; // "weight correction term", one for each synapse
public:
    virtual double get_stored_output() { return m_output; }
    double get_stored_error() { return m_error; }
    //  return synaptic weight at position pos (must not be called for bias units,
    //  since the vector m_synapses is empty for those)
    double operator[] (size_t pos) { return m_synapses[pos]; }
    size_t size() { return m_synapses.size(); }
}; //_____________________________________________________________________________

class bias_unit : public node {
// a bias unit goes in the zeroth position of the input layer and of the hidden layer.
// the output layer has no bias unit, since it does not "feed" a subsequent layer.
public:
    // a bias unit has a constant output, and no incoming connections.
    bias_unit () {  m_output = bias_value;  m_error = 0.0; }
    double get_stored_output() { return bias_value; }
}; //_____________________________________________________________________________

class neuron : public node {
// class for a binary neuron (output between 0 and 1), using single-precision doubles
protected:
    double randf();
    double weighted_sum(layer& l);
public:
    neuron() {}
    // the code that instantiates a neuron must call one of these two init functions,
    // but not both.
    void init(size_t sz);
    void init_NW(size_t sz, double beta);
    double output(layer& l);
    // simple accessor, does not allow changing the stored output value
    double get_stored_output() { return m_output; }

    // to be used for neurons in the output layer
    double output_error_term(double training_value);
    void set_output_correction(double training_value, layer& hidden);

    // to be used for neurons in the hidden layer
    double hidden_error_term(layer& output_layer, size_t pos);
    void set_hidden_correction(layer& output_layer, size_t pos, layer& input);
    void adjust_weights();
}; //________________________________________________________________________________

class target {
// just an array of double with methods for converting from other target
// representations into doubles
protected:
    // holds the desired output corresponding to a single input vector
    vector<double> m_desired_outputs;
public:
    target (size_t sz);
    void copy_ints(const vector<int>& vect);
    void copy_string(const string& str);
    double operator[] (const size_t i) const { return m_desired_outputs[i]; }
}; //______________________________________________________________________________

class hidden_layer : public layer {
protected:
    // the vector will contain one bias unit and many neurons
    vector<node> m_nodes;
public:
    // input_sz, the size of the input layer, must include the bias unit
    // that provides input to the hidden layer.
    hidden_layer(size_t hidden_sz, size_t input_sz);
    double operator[] (const size_t i) { return m_nodes[i].get_stored_output(); }
    size_t size() const { return m_nodes.size(); }
    // return number of synapses in each neuron of the hidden layer (skip bias unit)
    size_t get_neuron_size() { return ((neuron*) &m_nodes[1])->size(); }
    void forward (layer& inputs);
    void backpropagate(layer& input);
    void set_corrections (layer& output, layer& input);
    void adjust_weights();
    node* get_node_ptr (size_t i) { return &m_nodes[i]; }
    void dump();
    friend class neuron;
}; //_________________________________________________________________

class input_layer : public layer {
protected:
    vector<double> m_inputs;
public:
    input_layer(size_t sz);
    void copy_doubles(const vector<double>& vect);
    void copy_ints(const vector<int>& vect);
    void copy_string(const string& s);
    double operator[] (const size_t i) { return m_inputs[i]; }
    size_t size() const { return m_inputs.size(); }
    size_t get_neuron_size() { return 0; }  // input units have no synapses
    node* get_node_ptr (size_t i) { return 0; }
    void dump();
}; //___________________________________________________________________

class output_layer : public layer {
protected:
    vector<neuron> m_neurons;  // no bias unit, only neurons
public:
    // hidden_sz, the size of the hidden layer, must include the bias unit
    // that provides input to the output layer.
    output_layer(size_t output_sz, size_t hidden_sz);
    double operator[] (const size_t i) { return m_neurons[i].get_stored_output(); }
    size_t size() const { return m_neurons.size(); }

    // assuming that the doubles in the output are the bits of a char string
    void output_to_string(string& str);
    // return number of synapses in each neuron of the output layer (no bias unit here)
    size_t get_neuron_size() { return m_neurons[0].size(); }
    void forward (layer& inputs);
    void backpropagate(hidden_layer& hidden, input_layer& input, target& training_value);
    node* get_node_ptr (size_t i) { return &m_neurons[i]; }
    void adjust_weights();
    void set_corrections (layer& input, target& training_value);
    void dump();
    friend class neuron;
}; //_________________________________________________________________

typedef struct pair<string, string> string_pair;

class trainer {
// driver class that trains the network and then tests it
protected:
    int m_max_epochs;
    int error_count (input_layer& i, target& t, output_layer& o, hidden_layer& h, vector<string_pair>& string_pairs);
public:
    trainer (const int max_epochs) { m_max_epochs = max_epochs; }

    // return the number of epochs of training that actually occurred
    int train (output_layer& o, hidden_layer& h, input_layer& i, target& t, vector<string_pair>& string_pairs);

    int read_pairs (string& filename, vector<string_pair>& string_pairs);
    int read_tests (string& testing_file, vector<string>& test_data);
    void test (output_layer& o, hidden_layer& h, input_layer& i, target& t, vector<string>& test_data);
}; //________________________________________________________________________________________________________
/***********  END CLASS DECLARATIONS, BEGIN METHOD IMPLEMENTATIONS ***********/

double neuron::randf() {
// return random double between -0.5 and +0.5
    double r = ((double)rand())/double_rand_max;
    return r - 0.5;
} //__________________________________________________________________

void neuron::init(size_t sz) {
// insert random doubles between -0.5 and +0.5  into the vector.
// these are the synaptic weights.

    m_synapses.reserve(sz);
    for (size_t i=0; i<sz; i++)
        m_synapses.push_back(randf());

    m_correction.resize(sz);
} //__________________________________________________________________
void neuron::init_NW(size_t sz, double beta) {
// Nguyen-Widrow initialization for the synapses into the hidden units.
// this method is to be used only if you change the neuron to a
// bipolar output (-1 to +1) rather than the binary output (0 to 1).

    m_synapses.reserve(sz);

    // set weight for connection between this neuron and the input layer bias unit
    m_synapses.push_back(randf() * (beta/0.5));

    double accumulator = 0.0;
    for (size_t i=1; i<sz; i++) {
        double r = randf();
        m_synapses.push_back(r);
        accumulator += r*r;
    }
    double norm = sqrt(accumulator);
    for (size_t i=1; i<sz; i++)
        m_synapses[i] *= beta/norm;

    m_correction.resize(sz);
} //__________________________________________________________________

double neuron::weighted_sum(layer& l) {

    size_t sz = m_synapses.size();
    if (sz != l.size())
        throw "incorrect input size in neuron::output\n";

    double accumulator = 0.0;  // try this with double instead
    for (size_t i=0; i<sz; i++)
        accumulator += m_synapses[i] * l[i];
    // cout << "weighted sum=" << accumulator << "  ";
    return accumulator;
} //__________________________________________________________________

double neuron::output(layer& l) {
// compute the weighted sum of the inputs, then apply the binary sigmoid function

    m_output = 1.0 / (1.0 + exp(-1.0 * weighted_sum(l)));
    return m_output;
} //__________________________________________________________________

double neuron::output_error_term(double training_value) {
// sets the "error information term", as described in step 6 on
// page 295 of the FNN book, for neurons in the output layer
    m_error = (training_value - m_output) * (m_output - m_output*m_output);
    return m_error;
} //__________________________________________________________________

void neuron::set_output_correction(double training_value, layer& hidden) {
// returns the "weight correction term" vector (one value for each synapse of
// this neuron, as described in step 6 on page 295 of the FNN book

    size_t sz = m_synapses.size();
    if (sz != hidden.size())
        throw "vector size mismatch in neuron::output_correction_term.\n";

    for (size_t j=0; j<sz; j++)
        m_correction[j] = alpha * output_error_term(training_value) * hidden[j];
} //__________________________________________________________________

double neuron::hidden_error_term(layer& output, size_t j) {
    // j is the index of this neuron within the hidden layer

    // j must not exceed the number of synapses in each neuron of the output layer
    if (j >= output.get_neuron_size())
        throw "hidden neuron index exceeds synapse count in output layer.\n";

    // this is delta_in_j in step 7 of the algorithm in the FNN book
    double accumulator = 0.0;

    size_t sz = output.size();
    // compute the sum of the delta inputs from the output layer
    for (size_t k=0; k<sz; k++) {
        neuron* p_n = (neuron*) output.get_node_ptr(k);
        accumulator += p_n->get_stored_error() * p_n->m_synapses[j];
    }
    m_error = (double) accumulator * (m_output - m_output*m_output);
    return m_error;
} //___________________________________________________________________

void neuron::set_hidden_correction(layer& output_layer, size_t pos, layer& input) {
    // neuron::hidden_error_term must be called first
    m_error = hidden_error_term (output_layer, pos);
    size_t sz = m_correction.size();
    for (size_t i=0; i<sz; i++)
        m_correction[i] = alpha *  m_error * (m_output - m_output*m_output);

} //___________________________________________________________________

void neuron::adjust_weights () {
    size_t sz = m_synapses.size();
    for (size_t i=0; i<sz; i++)
        m_synapses[i] += m_correction[i];
} //___________________________________________________________________

/////////////////// end class neuron ////////////////////////////////////

input_layer::input_layer(size_t sz) {
    m_inputs.resize(sz+1); //  +1 for the bias unit
    m_inputs[0] = bias_value;
} //__________________________________________________________________

void input_layer::copy_doubles(const vector<double>& vect) {
    size_t sz = m_inputs.size() - 1;  // minus one because of the bias unit
    if (vect.size() != sz)
        throw "size mismatch in input_layer::copy_doubles.\n";
    // m_inputs[0] is the "bias unit" (just a constant double)
    for (size_t i=1; i<=sz; i++)
        m_inputs[i] = vect[i];
} //___________________________________________________________________

void input_layer::copy_ints(const vector<int>& vect) {
    size_t sz = m_inputs.size() - 1;  // minus one because of the bias unit
    if (vect.size() != sz)
        throw "size mismatch in input_layer::copy_ints.\n";
    // m_inputs[0] is the "bias unit" (just a constant double)
    for (size_t i=1; i<=sz; i++)
        m_inputs[i] = (double)vect[i];
} //___________________________________________________________________

void input_layer::copy_string(const string& str) {
// convert a string into an array of bits, each bit being represented by a double

    const size_t bits_per_char = 8;
    size_t sz = m_inputs.size() - 1; // -1 for the bias unit
    size_t len = str.length();

    if (bits_per_char * len != sz)
        throw "size mismatch in input_layer::copy_string.\n";

    for (size_t i=0; i<len; i++) {
        char c = str[i];
        for (size_t bit=0; bit<bits_per_char; bit++) {
            char mask = 1 << bit;
            // +1 for the bias unit
            m_inputs[i*bits_per_char + bit + 1] = double((c & mask) != 0);
        }
    }
} //___________________________________________________________________
void input_layer::dump() {
    // for debugging purposes...
    cout << "\nINPUT LAYER:\n";
    for (size_t i=0; i<m_inputs.size(); i++)
        cout << (*this)[i] << "  ";
} //___________________________________________________________________
/////////////  end implementation of input layer  //////////////////////

target::target (size_t sz) {
    m_desired_outputs.resize(sz);
} //____________________________________________________________________

void target::copy_ints(const vector<int>& vect) {
    size_t sz = m_desired_outputs.size();
    if (vect.size() != sz)
        throw "size mismatch in input_layer::copy_ints.\n";
    // m_inputs[0] is the "bias unit" (just a constant double)
    for (size_t i=0; i<sz; i++)
        m_desired_outputs[i] = (double)vect[i];
} //___________________________________________________________________

void target::copy_string(const string& str) {
    const size_t bits_per_char = 8;
    size_t sz = m_desired_outputs.size();
    size_t len = str.length();

    if (bits_per_char * len != sz)
        throw "size mismatch in input_layer::copy_string.\n";

    for (size_t i=0; i<len; i++) {
        char c = str[i];
        for (size_t bit=0; bit<bits_per_char; bit++) {
            char mask = 1 << bit;
            // +1 for the bias unit
            m_desired_outputs[i*bits_per_char + bit] = double((c & mask) != 0);
        }
    }
} //___________________________________________________________________
///////////////////////  end implementation of class target ////////////

hidden_layer::hidden_layer(size_t sz, size_t input_sz) {
    m_nodes.reserve(sz+1);  // plus one for the bias unit in this
    bias_unit b;
    m_nodes.push_back(b);

    // each neuron in the hidden layer must have a number of synapses equal to
    // the number of nodes in the input layer (including the bias unit in the
    // input layer)

    // the scale factor for Nguyen-Widrow initialization
    double beta = 0.7 * pow((double)sz, 1.0/(double)input_sz);

    // start at index 1, not zero, because the bias unit does not need to be
    // initialized.
    for (size_t i=1; i<=sz; i++) {
        neuron n;
        m_nodes.push_back(n);
        neuron* p_neuron = (neuron*) &m_nodes[i];
        // Nguyen-Widrow initialization is not used for the binary sigmoid output.
        // instead, random weights (-0.5 to +0.5) are used.
        // add +1 to hidden neurons' synapse count for connection to
        // the input layer bias unit.
           p_neuron->init(input_sz+1);
    }
} //__________________________________________________________________
void hidden_layer::forward (layer& inputs) {
// propagate activation forward (from input layer into this hidden layer)

    size_t sz = m_nodes.size();
    // start at index 1, not zero, because the bias unit does not need
    // to have its output calculated
    for (size_t i=1; i<sz; i++) {
        neuron* p_n = (neuron*) &m_nodes[i];
        p_n->output(inputs); // each neuron
    }
} //__________________________________________________________________

void hidden_layer::set_corrections (layer& output, layer& input) {

    size_t sz = m_nodes.size();
    // start at index 1, since the bias unit at index zero in this hidden layer
    // has no synapses.
    for (size_t j=1; j<sz; j++) {
        neuron* p_n = (neuron*)&m_nodes[j];
        p_n->set_hidden_correction(output, j, input);
    }
} //__________________________________________________________________

void hidden_layer::adjust_weights() {
    size_t sz = m_nodes.size();
    // no incoming connections on the bias unit, so skip zeroth node
    for (size_t j=1; j<sz; j++) {
        neuron* p_n = (neuron*)&m_nodes[j];
        p_n->adjust_weights();
    }
} //___________________________________________________________________

void hidden_layer::dump() {
// for debugging purposes...
    cout << "\nHIDDEN LAYER:\n";
    bias_unit* p_b = (bias_unit*) &(m_nodes[0]);
    cout << p_b->get_stored_output() << "  ";

    for (size_t i=1; i<m_nodes.size(); i++) {
        neuron* p_n = (neuron*) &(m_nodes[i]);
        cout << p_n->get_stored_output() << "  ";
    }
} //___________________________________________________________________
/////////////////////  end hidden layer implementation  //////////////

output_layer::output_layer(size_t output_sz, size_t hidden_sz) {
    m_neurons.resize (output_sz);  // no bias unit to add to this layer
    for (size_t i=0; i<output_sz; i++)
        m_neurons[i].init(hidden_sz+1);  // hidden layer has a bias unit, so +1
} //__________________________________________________________________

void output_layer::forward (layer& hidden) {
// propagate activation forward (from hidden layer into this output layer)

    size_t sz = m_neurons.size();
    // start at index 0, because there is no bias unit in this layer
    for (size_t i=0; i<sz; i++) {
        neuron* p_n = (neuron*) &m_neurons[i];
        p_n->output(hidden);
    }
} //__________________________________________________________________

void output_layer::set_corrections (layer& hidden, target& training_value) {
    size_t sz = m_neurons.size();
    // start at index 0, because there is no bias
    for (size_t k=0; k<sz; k++) {
        neuron& n = m_neurons[k];
        n.set_output_correction(training_value[k], hidden);
    }
} //___________________________________________________________________

void output_layer::adjust_weights() {
    size_t sz = m_neurons.size();
    // for all neurons in this output layer...
    for (size_t k=0; k<sz; k++)
        m_neurons[k].adjust_weights();
} //___________________________________________________________________

void output_layer::backpropagate (hidden_layer& hidden, input_layer& input, target& training_value) {

    set_corrections(hidden, training_value);
    hidden.set_corrections(*this, input);

    // step 8 in the FNN book
    adjust_weights();
    hidden.adjust_weights();

} //__________________________________________________________________

void output_layer::output_to_string(string& str) {
// if each double that is the output of a neuron in this layer is (approximately)
// the value of a bit in a character, then this function converts
// the vector of doubles into a character string using those bit values.

    str.clear();
    const size_t bits_per_char = 8;
    size_t sz = m_neurons.size();

    // sanity check
    if (sz % bits_per_char != 0)
        throw "output layer size is not a multiple of 8 in output_layer::get_string.\n";

    size_t char_count = sz / 8;
    for (size_t i=0; i<char_count; i++) {
        char c = 0;
        for (size_t bit=0; bit<bits_per_char; bit++) {
           char one_or_zero = m_neurons[i*bits_per_char + bit].get_stored_output() > 0.7 ? 1 : 0;
           c += one_or_zero << bit;
        }
        str += c;
    }
} //____________________________________________________________________________

void output_layer::dump() {
// for debugging purposes
    cout << "\nOUTPUT LAYER:\n";
    for (size_t i=0; i<m_neurons.size(); i++)
        cout << (*this)[i] << "  ";
} //____________________________________________________________________________
/////////////////// end output layer implementation ///////////////////////////////

int trainer::train (output_layer& o, hidden_layer& h, input_layer& i, target& t, vector<string_pair>& string_pairs) {
    int training_epoch = 0;

    // the forward propagation and the training happen during the call to "error_count"
    while (error_count(i, t, o, h, string_pairs)>0 && ++training_epoch<m_max_epochs)
        ;
    cout << "\nTRAINING EPOCHS: " << training_epoch << endl;
    return training_epoch;
} //______________________________________________________________________________________________________
int trainer::error_count (input_layer& i, target& t, output_layer& o, hidden_layer& h, vector<string_pair>& string_pairs) {

    int errors = 0;
    string curr_output;
    vector<string_pair>::iterator it = string_pairs.begin();
    while (it != string_pairs.end()) {
        i.copy_string(it->first);  //current input
        t.copy_string(it->second);  // desired output
        h.forward(i);
        o.forward(h);
        o.output_to_string(curr_output);
        if (curr_output != it->second) {
            errors++;
            o.backpropagate (h, i, t);
        }
        it++;  // go to next pair
    }
    return errors;
} //___________________________________________________________________________________________

int trainer::read_pairs (string& filename, vector<string_pair>& string_pairs) {
// the input file should be a series of string pairs, with a blank line after each pair,
// and each pair consisting of two left-justified strings on separate lines, as follows:
/*
input1
output1

input2
output2

input3
output3

*/

    ifstream infile(filename.c_str());
    string str_in, str_target, str_dummy;
    while (infile) {
        getline(infile, str_in);
        if (!infile)
            break;
        str_in.resize(input_layer_size/bits_per_char, ' ');
        getline(infile, str_target);
        if (!infile)
            break;
        str_target.resize(output_layer_size/bits_per_char, ' ');
        string_pair p = make_pair(str_in, str_target);
        string_pairs.push_back(p);
        getline(infile, str_dummy);
    }
    infile.close();
    return string_pairs.size();
} //____________________________________________________________________________________________
int trainer::read_tests (string& testing_file, vector<string>& test_data) {
// the test file is just a series of test strings to be used as network input.
// there is one test string per line, left-justified.

    ifstream infile(testing_file.c_str());
    string str_in;
    while (infile) {
        getline(infile, str_in);
        if (!infile)
            break;
        str_in.resize(input_layer_size/bits_per_char, ' ');
        test_data.push_back(str_in);
    }
    infile.close();
    return test_data.size();
} //____________________________________________________________________________________________

void trainer::test (output_layer& o, hidden_layer& h, input_layer& i, target& t, vector<string>& test_data) {
    vector<string>::iterator it = test_data.begin();
    string curr_output;
    while (it != test_data.end()) {
        i.copy_string(*it);  //current input
        h.forward(i);
        o.forward(h);
        o.output_to_string(curr_output);
        cout << *it << " \t" << curr_output << endl;
        it++;
    }

} //__________________________________________________________________________________________
///////////////////////////////// end class trainer /////////////////////////////////

int main() {

    srand(time(0));
    // how long to train the network, at most.  training will stop prior to
    // reaching this limit if the actual outputs match the target outputs.
    const int max_training_epochs =  3000;

    try {
        // the neurons in the output and hidden layers must each know the size of the
        // previous layer in order to have the right number of synapses per neuron.
        output_layer o (output_layer_size, hidden_layer_size);
        hidden_layer h (hidden_layer_size, input_layer_size);
        input_layer i (input_layer_size);
        // the target vector must be of the same size as the output layer
        target t (output_layer_size);

        trainer tr(max_training_epochs);
        vector<string_pair> training_data;
        vector<string> test_data;

        // CHANGE THESE TWO FILE PATHS TO MATCH YOUR OWN TRAINING AND TEST DATA
        string training_file = "/home/toucan/workspace/backprop/Debug/train.txt";
        string testing_file = "/home/toucan/workspace/backprop/Debug/test.txt";

        tr.read_pairs (training_file, training_data);
        tr.read_tests (testing_file, test_data);

        tr.train(o, h, i, t, training_data);
        tr.test(o, h, i, t, test_data);
    }
    catch (const char* msg) {
        cout << msg << endl;
    }
    return 0;
}


