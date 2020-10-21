Hello, 

Thanks for marking :)

As stated in the report, to run the program, run the main.py file with either of these arguments:
(For ease of use feel free to copy paste this lines and paste them into the terminal)

For training with MNIST data:
python main.py 784 30 10 TrainDigitX.csv.gz TrainDigitY.csv.gz TestDigitX.csv.gz PredictDigitY.csv.gz
or
python main.py 784 30 10 TrainDigitX.csv.gz TrainDigitY.csv.gz TestDigitX2.csv.gz PredictDigitY2.csv.gz

For training with the simple network from chapter one:
python main.py 2 2 2
