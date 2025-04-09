## Simple Method iterations

The method of simple iterations

Example:

`./SimpleIterations <epsilon> <alpha> <x0> <exOperator>(default="x-1/5*(2*x+cos(x)+(2*x)/(1+x*x))")`

```bash
./SimpleIterations 1e-5 0.86 1            
Parameters:
 - epsilon:     1e-05
 - alpha:       0.86
 - x0:          1
 - exOperator Phi[x]=x: x-1/5*(2*x+cos(x)+(2*x)/(1+x*x))=x
Results:
 - Apriori              = 88
 - Aposteriori          = 2.83761e-06
 - Computed steps       = 10
 - x                    = -0.249571
```

```bash 

./SimpleIterations 1e-5 0.86 1  "x-1/(3+sqrt(2))*(2*x-cos(x)+sin(x)+atan(x)-2)"
Parameters:
 - epsilon:     1e-05
 - alpha:       0.86
 - x0:          1
 - exOperator Phi[x]=x: x-1/(3+sqrt(2))*(2*x-cos(x)+sin(x)+atan(x)-2)=x
Results:
 - Apriori              = 81
 - Aposteriori          = 6.22443e-06
 - Computed steps       = 5
 - x                    = 0.726981
```
