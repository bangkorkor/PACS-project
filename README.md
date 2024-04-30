## TODO

- Implement phys_mod class
- Fix interpolation class, it is not working properly now I think
- Limit includes to make compiling faster
- is it possible to load modules in makefile?


## modules used

Eigen
boost
gsl

## run code

1. You need the right enviorment, make sure it is downloaded. To start and run, do: ```docker start pacs-env```, ```docker exec -it pacs-env /bin/bash``` 
2. Do ```module list``` to see loaded modules. Load the right modules with ```module load eigen```, ```module load boost```, ```module load gsl```
3. Navigate to ```/home/jammy/shared-folder/PACS-project``` and do ```make``` to compile. Remember to do ```make clean``` before if old executables are made
4. Do ```./build/main``` to execute 
