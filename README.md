LMD: Last-Mile Delivery Made Practical: An Efficient Route Planning Framework with Theoretical Guarantees
========================================================================

This repository stores the source code of the proposed algorithm to solve the Last-Mile Delivery (LMD) problem in the following paper.

[1] **Last-Mile Delivery Made Practical: An Efficient Route Planning Framework with Theoretical Guarantees.**
*Yuxiang Zeng, Yongxin Tong, Lei Chen.* PVLDB 13(3): 320-333 (2019). [link](http://www.vldb.org/pvldb/vol13/p320-zeng.pdf)

If you find this work helpful in your research, please consider citing our paper and the bibtex are listed below:
```  
@article{DBLP:journals/pvldb/ZengTC19,
  author    = {Yuxiang Zeng and Yongxin Tong and Lei Chen},
  title     = {Last-Mile Delivery Made Practical: An Efficient Route Planning Framework with Theoretical Guarantees},
  journal   = {{PVLDB}},
  volume    = {13},
  number    = {3},
  pages     = {320--333},
  year      = {2019},
}
```  



Usage of the algorithms
---------------

### Environment

gcc/g++ version: 7.4.0 

OS: Ubuntu

### Compile the algorithm

cd algorithm && make all

fesif: the FESI algorithm in the paper

chst: the algorithm of constructing an HST

### Run the algorithm

**1. First randomly construct several indexes of HST**  
cd ../dataset && python batchRunHST.py   
batchRunHST.py: a python script to construct the HSTs

**2. Run the FESI algorithm with the constructed HST**   
./fesif ./synData/location/mu100/loc_00.txt ./synData/6000_150_2_8_mu100/data_00.txt ./synHST/mu100/loc_00/hst_0000.txt
loc_00.txt: the locations of all the metric spaces  
data_00.txt: the information of the couriers and requests   
hst_0000.txt: our index, HST


**3. We also provide our python scripts to conveniently conduct all the experiments**  
cd ../dataset && python batchRunSyn.py   
batchRunSyn.py: a python script to run the experiments on synthetic datasets
cd ../dataset && python batchRunReal.py      
batchRunReal.py: a python script to run the experiments on olist datasets


Description of the data generator
---------------

### Environment

Python: 2.7

### Run the scripts

olistData: the data of our real dataset Olist

olistHST: some samples of our HSTs for the olist dataset

synData: some samples of our synthetic dataset

synHST: some samples of our HSTs for the synthetic dataset

genDataSyn.py: a script to generate the synthetic datasets in the experiments

batchRunHST.py: a python script to construct the HSTs

batchRunSyn.py: a python script to run the experiments on synthetic datasets

batchRunOlist.py: a python script to run the experiments on olist datasets


**Cainiao dataset**: For the cainiao dataset, please request for the dataset in the [website of Tianchi](https://tianchi.aliyun.com/competition/entrance/231581/introduction), as we do not have the permit to distribute the dataset.



Related resources
------------------------
We have maintained a paper list of the studies on the shared mobility service (e.g., ridesharing, food delivery and urban logistics). [link](https://github.com/BUAA-BDA/ridesharing-paperlist)



Presentation Slides
------------------------
VLDB20-LMD-github.ppsx is our presentation slides in VLDB 2020.


Contact
------------
- Yuxiang Zeng: yzengal@cse.ust.hk
- Yongxin Tong: yxtong@buaa.edu.cn

