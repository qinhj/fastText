## Quick Install ##
```
$ git clone https://github.com/facebookresearch/fastText.git
$ cd fastText

## with gcc
$ make

## with python (conda)
$ conda create -n fasttext python=3.6 -y
$ conda activate fasttext
(fasttext) $ pip install .  // setup.py
```

## Quick Data ##
```
$ wget https://dl.fbaipublicfiles.com/fasttext/data/cooking.stackexchange.tar.gz
$ tar xvzf cooking.stackexchange.tar.gz
## have a quick glance
$ head cooking.stackexchange.txt
$ wc cooking.stackexchange.txt
## quick preprocess
$ cat cooking.stackexchange.txt | sed -e "s/\([.\!?,'/()]\)/ \1 /g" | tr "[:upper:]" "[:lower:]" > cooking.preprocessed.txt
$ head -n 12404 cooking.preprocessed.txt > cooking.train
$ tail -n 3000 cooking.preprocessed.txt > cooking.valid

$ wget -c http://mattmahoney.net/dc/enwik9.zip -P data
$ unzip data/enwik9.zip -d data
$ perl wikifil.pl data/enwik9 > data/fil9
```

## Quick Start: Classification ##
```
## start with binrary
$ ./fasttext supervised -input data/cooking.train -output data/model_cooking -lr 1.0 -epoch 25 -wordNgrams 2 -bucket 200000 -dim 50 -loss hs
$ ./fasttext test data/model_cooking.bin data/cooking.valid

## start with python
$ python
## 导入fasttext
>>> import fasttext
## 使用fasttext的train_supervised方法进行文本分类模型的训练
>>> model = fasttext.train_supervised(input="cooking.train")
## 使用模型预测一段输入文本, 通过我们常识, 可知预测是正确的, 但是对应预测概率并不大
>>> model.predict("Which baking dish is best to bake a banana bread ?")
## 元组中的第一项代表标签, 第二项代表对应的概率
(('__label__baking',), array([0.06550845]))
## 为了评估模型到底表现如何, 我们在3000条的验证集上进行测试
>>> model.test("cooking.valid")
## 元组中的每项分别代表, 验证集样本数量, 精度以及召回率
(3000, 0.124, 0.0541)
## 设置train_supervised方法中的参数epoch来增加训练轮数, 默认的轮数是5次
## 设置train_supervised方法中的参数lr来调整学习率, 默认的学习率大小是0.1
## 增大学习率意味着增大了梯度下降的步长使其在有限的迭代步骤下更接近最优点
## 设置train_supervised方法中的参数wordNgrams来添加n-gram特征, 默认是1, 也就是没有n-gram特征
## 设置train_supervised方法中的参数loss来修改损失计算方式(等效于输出层的结构), 默认是softmax层结构
>>> model = fasttext.train_supervised(input="cooking.train", lr=1.0, epoch=25, wordNgrams=2, loss='hs')
## 手动调节和寻找超参数是非常困难的, 因为参数之间可能相关, 并且不同数据集需要的超参数也不同,
## 因此可以使用fasttext的autotuneValidationFile参数进行自动超参数调优.
## autotuneValidationFile参数需要指定验证数据集所在路径, 它将在验证集上使用随机搜
索方法寻找可能最优的超参数.
## 使用autotuneDuration参数可以控制随机搜索的时间, 默认是300s
>>> model = fasttext.train_supervised(input='cooking.train', autotuneValidationFile='cooking.valid', autotuneDuration=600)
## 针对多标签多分类问题, 使用'softmax'或者'hs'有时并不是最佳选择, 因为我们最终得到的应该是多个标签, 而softmax却只能最大化一个标签.
## 所以我们往往会选择为每个标签使用独立的二分类器作为输出层结构,
## 对应的损失计算方式为'ova'表示one vs all.
## 这种输出层的改变意味着我们在统一语料下同时训练多个二分类模型,
## 对于二分类模型来讲, lr不宜过大, 这里我们设置为0.2
>>> model = fasttext.train_supervised(input="cooking.train", lr=0.2, epoch=25, wordNgrams=2, loss='ova')
## 使用model的save_model方法保存模型到指定目录
>>> model.save_model("./model_cooking.bin")
## 使用fasttext的load_model进行模型的重加载
>>> model = fasttext.load_model("./model_cooking.bin")
```

## Quick Start: Word Embedding ##
```
## start with binary
$ mkdir result
$ ./fasttext skipgram -input data/fil9 -output result/fil9 -minn 2 -maxn 5 -dim 300 -epoch 1 -lr 0.1 -thread 4
$ ./fasttext cbow -input data/fil9 -output result/fil9.cbow
$ head -n 4 result/fil9.vec
$ echo "asparagus pidgey yellow" | ./fasttext print-word-vectors result/fil9.bin
$ ./fasttext nn result/fil9.bin
$ ./fasttext analogies result/fil9.bin
## other commands:
$ ./fasttext print-word-vectors model.bin < queries.txt
## train supervised classify model
$ ./fasttext supervised -input train.txt -output model
## test
$ ./fasttext test model.bin test.txt 1
## k tags (with probility)
$ ./fasttext predict model.bin test.txt k
$ ./fasttext predict-prob model.bin test.txt k
## get sentence vectors
$ ./fasttext print-sentence-vectors model.bin < text.txt
## reduce memory
$ ./fasttext quantize -output model

## start with python
$ python
## 导入fasttext
>>> import fasttext
## 使用fasttext的train_unsupervised(无监督训练方法)进行词向量的训练
## 它的参数是数据集的持久化文件路径'data/fil9'
>>> model = fasttext.train_unsupervised('data/fil9')
## 通过get_word_vector方法来获得指定词汇的词向量
>>> model.get_word_vector("the")
## 在训练词向量过程中, 我们可以设定很多常用超参数来调节我们的模型效果, 如:
## 无监督训练模式: 'skipgram'(默认) 或者 'cbow' (实践中skipgram模式在利用子词方面比cbow更好)
## 词嵌入维度dim: 默认为100, 但随着语料库的增大, 词嵌入的维度往往也要更大.
## 数据循环次数epoch: 默认为5, 但当你的数据集足够大, 可能不需要那么多次.
## 学习率lr: 默认为0.05, 根据经验, 建议选择[0.01，1]范围内.
## 使用的线程数thread: 默认为12个线程, 一般建议和你的cpu核数相同.
>>> model = fasttext.train_unsupervised('data/fil9', "cbow", dim=300, epoch=1, lr=0.1, thread=4)
## 查找"运动"的邻近单词, 我们可以发现"体育网", "运动汽车", "运动服"等.
>>> model.get_nearest_neighbors('sports')
[(0.8414610624313354, 'sportsnet'), (0.8134572505950928, 'sport'), (0.8100415468215942, 'sportscars'), (0.8021156787872314, 'sportsground'), (0.7889881134033203, 'sportswomen'), (0.7863013744354248, 'sportsplex'), (0.7786710262298584, 'sporty'), (0.7696356177330017, 'sportscar'), (0.7619683146476746, 'sportswear'), (0.7600985765457153, 'sportin')]
## 查看前100个词汇(这里的词汇是广义的, 可以是中文符号或汉字))
>>> model.words[:100]
```

## Quick Reference ##
[fastText](https://fasttext.cc)  
[FastText 中文教程](https://blog.csdn.net/weixin_43891607/article/details/106903132)