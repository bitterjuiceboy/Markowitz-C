# Markowitz-C

Modern Portfolio Theory - MPT A theory on how risk-averse investors can construct portfolios to optimize or maximize expected return based on a given level of market risk, emphasizing that risk is an inherent part of higher reward. “Modern Portfolio Theory - MPT.” 


  $$Mean(Returns) = \frac{1}{n} \sum_{i=1}^{n}r_i$$
  $$Var(Returns) = \sum_{i=1}^{n}(r_i-\mu)^2$$
  $$Mean(Portfolio\ Returns) = \sum_{i=1}^{n}w_i r_i$$
  $$Var(Portfolio\ Returns) = \sum_{i=1}^{n}\sum_{j=1}^{n}w_i w_j \rho_{i,j} \sigma_i \sigma_j = \mathbf{w}^T \bullet \big(\mathbf{cov} \bullet \mathbf{w}\big)$$

#### Description
为了方便跨平台运行的C语言实现的Markowitz组合优化程序。
尽可能避免使用花哨的小众的库，尽可能少的依赖。

#### Features
- 计算
    - [x] 平均回报 mean return
    - [x] 波动率(标准差) volatility，std
    - [x] 协方差矩阵 cov-matrix
    - [x] 组合收益、波动率、夏普比率 portfolio return，volatility， sharp ratio
- 优化
    - [x] MonteCarlo simulation
        - 随机 assets weights的方式，得到大量的组合。可以从中找到最优回报，最优夏普比率，最低波动等的组合权重。
        - [x] max return
        - [x] max sharp ratio
        - [x] min volatility
        - [x] efficient frontier
    - [x] numerical optimize algorithms
        - [ ] example: L-BFGS
- To do list
    - 功能方面
        - [x] 在MonteCarlo的基础上，实现寻找efficient frontier
        - [x] 在MonteCarlo的基础上，增加函数，输入给定的std，给出最大return的组合权重
        - [x] 在MonteCarlo的基础上，增加函数，输入给定的return， 给出最小std的组合权重
        - [x] 当前的寻找最优组合的方式是MonteCarlo模拟，选一种数值算法实现，寻找最优回报，最优sharp ratio，最小波动等组合。以及efficient frontier
    - 可视化
        - [ ] 探索尽可能轻量级的C语言的可视化方案(为了便于跨平台)。可以参考plot.ipynb里的效果图。
    - 代码优化
        - 当前demo的输出是通过printf函数输出到terminal的csv形式，通过 ./build/main path num_simulations > result.txt的形式输出到文件。比较简陋。可以做的优化点如下：
        - [x] 设计数据结构来存储result。更方便通过排序等方式寻找efficient frontier、maximum sharp ratio和输出到文件。
        - [x] 实现写csv文件。
        - [ ] 其他的可以优化的点也随时欢迎提出来。

#### Software Architecture
Software architecture description

#### Installation

1.  xxxx
2.  xxxx
3.  xxxx

#### Instructions

1.  xxxx
2.  xxxx
3.  xxxx

#### Contribution

1.  Fork the repository
2.  Create Feat_xxx branch
3.  Commit your code
4.  Create Pull Request

