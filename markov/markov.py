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
            sline = re.sub(r'\s*([,.;()]+)(\s+|$)', ' \\1 ', line.strip())
            words = re.split('\s', sline)
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
        space = ''
        while word_count < max_words:
            probs = self.probs[prev]

            r = random()

            word = ''
            for tpl in probs:
                prob = tpl[0]
                if r <= prob:
                    word = tpl[1]
                    break

            width += len(word)
            empty = (word is '')
            newline = (width > 70)

            if re.match(r'[,.;)]',word):
                space = ''

            elif empty:
                space = '\n\n'
                width = 0

            elif newline:
                space = '\n'
                width = 0

            sys.stdout.write("%s%s"%(space,word))

            space = ' '
            if empty or re.match(r'[(]',word):
                space = ''

            prev = word
            word_count += 1

        sys.stdout.write("\n")

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
