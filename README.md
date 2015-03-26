# Mutual-exclusion
Mutual exclusion tools comparison

### Without synchronization
~~~
time ./pthread 
Counter value 180232
Took me 0.146035 seconds

real	0m0.113s
user	0m0.149s
sys	0m0.000s
~~~

### With mutex
~~~
time ./pthread -m
Counter value 0
Took me 3.668629 seconds

real	0m2.064s
user	0m2.501s
sys	0m1.169s
~~~
