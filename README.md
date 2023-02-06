# C++ Neural Network from Scratch

* only uses standard libraries (no matrix operation or ML libraries)
* features ability to add any desirable amount of layers of any desirable size

``` Model my_model(input_vector_size = 784, loss_type = "CCE", learning_rate = 0.001); ```

``` my_model.add_layer(layer_size = 128); ```

* adjustable learning rate
* trained / tested on MNIST dataset with greater than 92% test accuracy

## File Breakdown:

- Main.cpp -> loads MNIST data, builds, trains, and tests model
- functions.h -> holds misc. functions for data processing
- Model.h -> holds Model class. Members hold representations of the model, perform forward / backward passes, and more
- matrix.h -> holds classes necessary for certain matrix (nested vector) operations

