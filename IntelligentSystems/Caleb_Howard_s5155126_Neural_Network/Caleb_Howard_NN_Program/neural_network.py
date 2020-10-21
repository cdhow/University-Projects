import numpy as np


# Sigmoid utility function
def sigmoid(x):
    return 1 / (1 + np.exp(-x))


# Sigmoid derivative utility function
def sigmoid_prime(x):
    return (np.exp(-x)) / ((np.exp(-x) + 1) ** 2)


class NeuralNetwork():
    def __init__(self, sizes, epochs=30, l_rate=3.0, batch_size=20, params=None):
        """
        :param sizes: number of neurons in the respective layers or the NN
        :param epochs: iterations over sample data
        :param batch_size: size of each mini batch
        :param l_rate: learning rate
        :param params: optional user defined weights and biases instead of random
        """
        self.sizes = sizes
        self.epochs = epochs
        self.l_rate = l_rate
        self.batch_size = batch_size
        # list that contains the accuracy of the network per epoch
        self.accuracy_per_epoch = []
        # list that contains the quadratic cost of the network per epoch
        self.quadratic_cost_per_epoch = []
        # list that contains the cross entropy cost of the network per epoch
        self.cross_entropy_per_epoch = []

        # We use dictionaries to store our network parameters
        self.weights, self.biases = self.init_params(params)

    def init_params(self, param):
        # number of nodes in each layer
        input_layer = self.sizes[0]
        hidden = self.sizes[1]
        output_layer = self.sizes[2]
        if param:
            # User has passed optional weights and biases
            w1, w2, b1, b2 = param
            weights = {'W1': w1, 'W2': w2}
            biases = {'B1': b1, 'B2': b2}
        else:
            # # Initialise biases and weight, randomly select them
            # from a standard normal distribution (mean 0, variance 1)
            weights = {
                'W1': np.random.randn(hidden, input_layer) * np.sqrt(1. / hidden),
                'W2': np.random.randn(output_layer, hidden) * np.sqrt(1. / output_layer),
            }
            biases = {
                'B1': np.random.randn(hidden) * np.sqrt(1. / hidden),
                'B2': np.random.randn(output_layer) * np.sqrt(1. / output_layer),
            }

        return weights, biases

    def forward_pass(self, x):
        """
        This method takes the input x and calculates the Net Input
        for each neuron (Z), it then uses the sigmoid function to
        squash the Net Inputs and give us the activation for each neuron (A)
        The method returns an array of activations of the output layer
        """
        weights = self.weights
        biases = self.biases

        # Sample data is the activations for the input layer
        weights['A0'] = x

        # Calculation of netIn (Z) for the input layer to the hidden layer
        # dot produce of weights and associated activations + bias
        weights['Z1'] = np.dot(weights["W1"], weights['A0']) + biases['B1']
        # Squash the netIn (Z) with sigmoid to get the activation (A).
        # This activation will be used in the next layer to calculate
        # the net input of the next layer neurons
        weights['A1'] = sigmoid(weights['Z1'])

        # Repeat above for hidden layer to output layer with activations from previous
        # calculation
        weights['Z2'] = np.dot(weights["W2"], weights['A1']) + biases['B2']
        weights['A2'] = sigmoid(weights['Z2'])

        # Return the activations of the output layer
        return weights['A2']

    def backward_pass(self, y, output):
        """
        Takes the desired y result and the output of our forward pass
        and gives us the partial derivative of each weight with
        respect to the total error
        """
        weights = self.weights
        # For storing partial derivatives of weights and biases
        # with respect to total error
        delta_b = {}
        delta_w = {}

        # Calculate W2 and B2 update
        # First Calculate the total error cost derivatives,
        # derivative of (output layer minus target layer all squared)
        # for each neuron is -(target - activation) = (activation - target)
        # We then multiple that error with the partial derivatives of the output
        # layer activations with respect to the net inputs of the output layer
        # This can be achieved by passing the net inputs into sigmoid prime.
        error = (output - y)  # shape = (output_layer,)
        n_output_nodes = self.sizes[2]
        n_hidden_nodes = self.sizes[1]
        n_input_nodes = self.sizes[0]

        d_z2 = error * sigmoid_prime(weights['Z2'])  # element-wise multiplication
        delta_b['B2'] = d_z2 * 1  # Biases is error * 1 as biases neurons = 1

        d_z2 = d_z2.reshape(n_output_nodes, 1)

        # For the weights we use the activation of the hidden layer
        delta_w['W2'] = np.dot(d_z2, weights['A1'].reshape(1, n_hidden_nodes))

        # Repeat Calculation for W1 and B1 with respect to the previous calculated dz_2
        error = np.dot(weights['W2'].T, d_z2)
        d_z1 = error * sigmoid_prime(weights['Z1']).reshape(n_hidden_nodes, 1)
        delta_b['B1'] = d_z1.reshape(n_hidden_nodes, ) * 1
        delta_w['W1'] = np.dot(d_z1, weights['A0'].reshape(1, n_input_nodes))

        return delta_w, delta_b

    def update_params(self, delta_w_sum, delta_b_sum):
        """
        Update network biases and weights using
        Stochastic Gradient Descent.
        """
        # For each weight take the calculated partial derivative sum, divide it
        # by batch size to get the average then multiple it by the learning rate,
        # finally, minus that value from the current weight to get our new weights.
        for key, value in delta_w_sum.items():
            self.weights[key] -= (self.l_rate / self.batch_size) * value

        # Repeat the same for the biases
        for key, value in delta_b_sum.items():
            self.biases[key] -= (self.l_rate / self.batch_size) * value

    def evaluate_test_data(self, x_test, y_test):
        """
        This method will run the test data through the
        neural network and calculate the quadratic cost
        for the output.
        It also calculates the accuracy of the network
        """
        predictions = []
        partial_q_cost = np.zeros(self.sizes[-1])
        partial_ce_cost = np.zeros(self.sizes[-1])
        for x, y in zip(x_test, y_test):
            # Run x through our network
            output = self.forward_pass(x)
            p = np.argmax(output)
            y_digit = np.argmax(y)
            predictions.append((p, y_digit))
            partial_q_cost += (output - y) ** 2
            partial_ce_cost += (y * np.log(output) + (1 - y) * np.log(1 - output))

        # Quadratic cost
        q_cost = np.sum(partial_q_cost) * (1 / (2 * len(x_test)))
        self.quadratic_cost_per_epoch.append(q_cost)
        # Cross Entropy cost
        ce_cost = np.sum(partial_ce_cost) * (1 / len(x_test))
        self.cross_entropy_per_epoch.append(ce_cost)
        # Prediction accuracy
        accuracy = sum(int(x == y) for (x, y) in predictions) / len(predictions)
        self.accuracy_per_epoch.append(accuracy)
        return q_cost, ce_cost, accuracy

    def initialise_mini_batches(self, training_data):
        return [training_data[i:i + self.batch_size]
                for i in range(0, len(training_data), self.batch_size)]

    def SGD(self, x_train, y_train, x_test=None, y_test=None):
        """
        :param x_train: input sample data
        :param y_train: desired output of the x_train input
        Optional:
        :param x_test: input sample data that is used to test the accuracy of the
        neural network for each epoch iteration
        :param y_test: the desired output of the x_test input
        """
        print("Training Neural Network with hyper-params: l_rate={}, batch_size={}, epochs={},"
              " sizes={}.".format(self.l_rate, self.batch_size, self.epochs, self.sizes))

        # Split the training set into batches of size batch_size
        batches = self.initialise_mini_batches(list(zip(x_train, y_train)))

        for i_epoch in range(self.epochs):
            for i_batch in range(len(batches)):
                # Get all samples in the current batch
                current_batch = batches[i_batch]
                # Summed errors of the weights and biases of the current batch
                summed_delta_w = {'W1': np.zeros((self.sizes[1], self.sizes[0])),
                                  'W2': np.zeros((self.sizes[2], self.sizes[1]))}
                summed_delta_b = {'B1': np.zeros(self.sizes[1]),
                                  'B2': np.zeros(self.sizes[2])}
                for x, y in current_batch:
                    # Feed input through network
                    output = self.forward_pass(x)
                    # Back propagate to get the partial derivatives
                    delta_w, delta_b = self.backward_pass(y, output)
                    # Sum the current weight errors
                    for key, val in delta_w.items():
                        summed_delta_w[key] += val
                    # Sum the current bias errors
                    for key, val in delta_b.items():
                        summed_delta_b[key] = np.add(summed_delta_b[key], val)

                # After the batch use the summed errors to update the
                # network params
                self.update_params(summed_delta_w, summed_delta_b)

            # If we have test data, compute the network accuracy and quadratic cost,
            # else print the weights after each epoch
            if x_test is not None:
                q_cost, ce_cost, accuracy = self.evaluate_test_data(x_test, y_test)
                print('Epoch: {}, Quadratic cost: {}, Cross Entropy cost: {}, '
                      'Accuracy: {}'.format(i_epoch + 1, q_cost, ce_cost, accuracy))
            else:
                self.print_params()
        print("\n")

    def predict(self, x_test):
        """
        :param x_test: test samples that will be feed through
        the network
        :return: vector of predictions corresponding to the input
        samples
        """
        predictions = []
        for x in x_test:
            # Run x through our network
            output = self.forward_pass(x)
            # Neuron with highest probability
            p = np.argmax(output)
            predictions.append(p)
        return predictions

    def print_params(self):
        """
        This method prints the weights and
        biases of the network.
        """
        count = 1
        for l in ['W1', 'W2']:
            for i in range(len(self.weights[l])):
                for w_arr in self.weights[l]:
                    print("w{}: {}".format(count, w_arr[i]))
                    count += 1
        for l in ['B1', 'B2']:
            for b in self.biases[l]:
                print("w{}: {}".format(count, b))
                count += 1
