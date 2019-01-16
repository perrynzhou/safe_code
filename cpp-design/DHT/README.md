
## run result
- ./test_dht  3 4 10000
```
./test_dht  {node_size}  {thread_size} {running_count_per_thread}
```
```
node 1 ,token start=0,end=536870910
node 2 ,token start=536870911,end=1073741821
node 3 ,token start=1073741822,end=1610612732
node 4 ,token start=1610612733,end=2147483643
node 5 ,token start=2147483644,end=2684354554
node 6 ,token start=2684354555,end=3221225465
node 7 ,token start=3221225466,end=3758096376
node 8 ,token start=3758096377,end=4294967287
....cluster nodes ,each token 536870911...
.........8 thread start.........
.........8 thread exit.........
cluster status:max_hash_key=4294967038,total=7280000,success:7280000
 ## node 1 handle request 859729
 ## node 2 handle request 846569
 ## node 3 handle request 839936
 ## node 4 handle request 822152
 ## node 5 handle request 1169656
 ## node 6 handle request 1054525
 ## node 7 handle request 836805
 ## node 8 handle request 850628
...destroy cluster...
```