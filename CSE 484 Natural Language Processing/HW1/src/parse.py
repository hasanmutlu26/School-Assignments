# This is the program to parse the entire book texts into documents made of 400 sentences at most.

import os
import nltk.data

# Names of txt files to be parsed.
booknames = ["biyoloji", "cagdas", "din", "edebiyat1", "edebiyat2", "edebiyat3", "edebiyat4", 
"edebiyat5", "edebiyat6", "edebiyat7", "edebiyat8", "felsefe1", "felsefe2", "inkilap", "psikoloji1", 
"psikoloji2", "sosyoloji1", "sosyoloji2", "tarih1", "tarih2", "tarih3", "tarih4", "tarih5"]


tokenizer = nltk.data.load('tokenizers/punkt/turkish.pickle')

os.makedirs("booktexts", exist_ok=True)

for k in range (len(booknames)):
    fp = open("../books/" + booknames[k] + ".txt")  ## Path of where the books text files are located.
    data = fp.read()
    dat2 = tokenizer.tokenize(data)

    dat1 = [0] * 400

    filecount = 0
    count = 0
    for i in range (len(dat2)):
        dat1[count] = dat2[i].replace("\n", " ")
        count += 1
        if(count == 400):
            count = 0
            filecount += 1
            while('' in dat1):
                dat1.remove('')
            fp2 = open("booktexts/"+ booknames[k] + "Out" + str(filecount) + ".txt", "w")
            fp2.write('\n'.join(dat1))
            for j in range (400):
                dat1[j] = ""

    filecount += 1
    fp2 = open("booktexts/"+ booknames[k] + "Out" + str(filecount) + ".txt", "w")
    fp2.write('\n'.join(dat1))


