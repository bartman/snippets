#!/usr/bin/env python

from collections import defaultdict
from collections import OrderedDict
from random import random
import sys
import re

def tree():
    return defaultdict(tree)

class markov_chain:
    def __init__(self):
        self.total = defaultdict(int)
        self.chain = defaultdict(lambda:defaultdict(int))
        self.probs = defaultdict(list)

    def read(self, file_name):
        f = open(file_name, 'r')
        contents = f.readlines()
        prev = ''
        for line in contents:
            words = re.split('\s', line)
            for word in words:
                if prev is word and prev is '':
                    continue
                self.total[prev] += 1
                self.chain[prev][word] += 1
                prev = word

        for prev in self.chain:
            total = self.total[prev]
            prob = 0.0
            probs = []
            for word in self.chain[prev]:
                count = self.chain[prev][word]

                prob += float(count) / total
                probs.append( (prob, word) )
            self.probs[prev] = probs

    def dump_raw(self):
        for prev in self.chain:
            ttl = self.total[prev]
            print "'%s' (%u)"%(prev, ttl)
            for word in self.chain[prev]:
                cnt = self.chain[prev][word]
                print "    (%u/%u) '%s'"%(cnt, ttl, word)

    def dump_prob(self):
        for prev in self.probs:
            for tpl in self.probs[prev]:
                prob = tpl[0]
                word = tpl[1]
                print "'%s' %f '%s'"%(prev, prob, word)

    def gen(self, max_words):
        word_count = 0
        width = 0
        prev = ''
        while word_count < max_words:
            probs = self.probs[prev]

            r = random()

            word = ''
            #for prob, word in probs.items():
            for tpl in probs:
                prob = tpl[0]
                if r <= prob:
                    word = tpl[1]
                    break

            width += len(word)
            newline = width > 70
            if newline or word == '':
                sep = '\n'
                width = 0
            else:
                sep = ' '

            sys.stdout.write("%s%s"%(word,sep))

            prev = word
            word_count += 1

mc = markov_chain()

for file_name in sys.argv[1:]:
    print >> sys.stderr, "reading %s"%file_name
    mc.read(file_name)

#print '-----'
#mc.dump_raw()
#print '-----'
#mc.dump_prob()
print '-----'
mc.gen(100)
