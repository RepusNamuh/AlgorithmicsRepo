import csv
import random
import numpy as np
def csvfile(file):
    csvdata = []
    with open(file,'r') as file:
        reader = csv.reader(file)
        for row in reader:
        # print(row)
            csvdata.append(row)
    return csvdata
def makeAfold(foldrows,data,rows,columns):
    randomrows = []
    while (len(randomrows) < foldrows) and (foldrows < len(data)):
    # get random rows from data for training
        rrow = random.randint(1,rows)
        # print(rrow)
        line = data[rrow]
        if (len(line) < columns):
            line.append(rrow)
            randomrows.append(line)
    return randomrows
## Dot Product Calculation
def predict(row, weight):
    activation = float(weight[2])
    for i in range(0, 2):
        activation = activation + weight[i]*float(row[i])
    if activation > 0:
        return 1.0
    else:
        return 0.0
def trainWeights(thisData,learningRate,nepochs,weight):
    for i in range(nepochs):
        for k in range(len(thisData)):
            thisrow = thisData[k]
            actual = float(thisrow[2]) # 0=Class 1, 1=Class 2
            prediction=predict(thisrow, weight)
            error = actual - prediction
            weight[2]=weight[2]+learningRate*error
            weight[0]=weight[0]+learningRate*error*float(thisrow[0])
            weight[1]=weight[1]+learningRate*error*float(thisrow[1])
            print(weight) # watch the weight vector adjusting
    
    return weight

trainingData = csvfile('Others/dragonSwimFeatures.csv')
foldData = makeAfold(30,trainingData,99,5)
testData = makeAfold(10, trainingData,99,5)
## What does this do?
weight=[0.0,0.0,0.0] # Initialise the weight vector
weight = trainWeights(foldData,0.1,100,weight)
print('after training weight = ',weight)

while True:
    answer = input("What row of data do you want to test: ")
    print(predict(trainingData[int(answer)], weight))
    answer = input("Do you want to continue Y/N: ")
    if answer.upper() == "Y":
        continue
    else:
        break
