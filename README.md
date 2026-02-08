# Kitsune - C kit, with useful types, data structures, algorithms...

## Use

### Run tests/examples

```c
make help
```

### Link in another project
1. Create a standard CMakeLists.txt;
2. Be sure to either clone the repo manually or use FetchContent;
3. Add a target;
4. Link the libraries you want:
```c
target_link_libraries(target PRIVATE Kitsune::LinkedList)
```

## State
- Core: 100%
    1. log: 100%
- Utils: 25%
    1. bit: 100%
    2. optional: 0%
    3. result: 0%
    4. slice: 0%
- Types: 50%
    1. number: 100%
    2. string: 0%
- Mem: 0%
    1. smart pointer: 0%

        1.1 unique: 0%
    2. arena: 0%
    3. pool: 0%
- Data: 0%
    1. array: 0%
    2. list: 0%

        2.1 array list: 0%

        2.2 linked list: 0%

        2.3 doubly linked list: 0%

        2.4 circular linked list: 0%

        2.5 circular doubly linked list: 0%
    3. tree: 0%
    
        3.1 binary tree: 0%
        
        3.3 k-ary tree: 0%
        
        3.2 search tree: 0%
        
        3.4 rb tree: 0%
        
        3.5 heap: 0%
    4. map: 0%
    
        4.1 hash map: 0%
        
        4.2 tree map: 0%
    5. set: 0%
    
        5.1 hash set: 0%
        
        5.2 tree set: 0%
    6. graph: 0%
    
        6.2 sparse graph: 0%
        
        6.1 dense graph: 0%
    7. adt: 0%
    
        7.1 ring: 0%
        
        7.2 stack: 0%
        
        7.3 queue: 0%
        
        7.4 deque: 0%
        
        7.5 priority queue: 0%
- Math: 0%
    1. algebra: 0%
    
        1.1 vec: 0%
        
        1.2 mat: 0%