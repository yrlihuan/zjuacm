#include <cstdio>
#include <cstring>
//#define TRIE1
#define TRIE2
#define INPUT_METHOD1
//#define INPUT_METHOD2

#if defined(TRIE1)
  typedef struct TrieNode
  {
    int next[26];
    bool word;
  }TrieNode;
#elif defined(TRIE2)
  typedef struct TrieNode
  {
    int right;
    int child;
    char character;
    bool word;
  }TrieNode;
#endif

TrieNode trieNode[350000];
int trieNodeNumber = 1;
char encryptedWord[100][30];
int encryptedWordLength[100];
bool matched[100];
int correctNumber[100];
int encryptedWordNumber;
char key[125];
char answer[128];
bool used[128];
bool findAnswer;
int wordNumber, encryptedNumber;

#if defined(TRIE1)
  void addWordToTrie(TrieNode &node, const char *word)
  {
    if(*word)
    {
      if(node.next[*word - 'A'])
      {
        addWordToTrie(trieNode[node.next[*word - 'A']], word + 1);
      }
      else
      {
        node.next[*word - 'A'] = ++trieNodeNumber;
        addWordToTrie(trieNode[trieNodeNumber], word + 1);
      }
    }
    else
    {
      node.word = true;
    }
  }
#elif defined(TRIE2)
  void addWordToTrie(int trieIndex, const char *word)
  {
    if(trieNode[trieIndex].character)
    {
      if(trieNode[trieIndex].character == *word)
      {
        if(!*(word+1))
        {
          trieNode[trieIndex].word = true;
        }
        else
        {
          if(!trieNode[trieIndex].child)
          {
            trieNode[trieIndex].child = ++trieNodeNumber;
          }
          addWordToTrie(trieNode[trieIndex].child, word + 1);
        }
      }
      else
      {
        if(trieNode[trieIndex].right)
        {
          addWordToTrie(trieNode[trieIndex].right, word);
        }
        else
        {
          trieNode[trieIndex].right = ++trieNodeNumber;
          addWordToTrie(trieNode[trieIndex].right, word);
        }
      }
    }
    else
    {
      trieNode[trieIndex].character = *word;
      if(!*(word+1))
      {
        trieNode[trieIndex].word = true;
      }
      else
      {
        trieNode[trieIndex].child = ++trieNodeNumber;
        addWordToTrie(trieNode[trieIndex].child, word + 1);
      }
    }
  }
#endif

#if defined(TRIE1)
  void printTrie(TrieNode &node, int depth)
  {
    for(int i=0;i<26;++i)
    {
      if(node.next[i])
      {
        for(int j=0;j<depth;++j)
        {
          printf(" ");
        }
        printf("%c\n", i + 'A');
        printTrie(trieNode[node.next[i]], depth + 1);
      }
    }
  }

  void printTrieAnother()
  {
    for(int i=0;i<=trieNodeNumber;++i)
    {
      printf("%2d ", i);
      for(int j=0;j<26;++j)
      {
        if(trieNode[i].next[j])
        {
          printf("%c%-2d", j + 'A', trieNode[i].next[j]);
        }
        else
        {
          printf("  ");
        }
      }
      if(trieNode[i].word)
      {
        printf(" WORD");
      }
      printf("\n");
    }
  }
#elif defined(TRIE2)
  void printTrie(int trieIndex, int depth)
  {
    for(int i=0;i<depth;++i)
    {
      printf(" ");
    }
    printf("%c\n",trieNode[trieIndex].character);
    if(trieNode[trieIndex].child)
    {
      printTrie(trieNode[trieIndex].child, depth + 1);
    }
    if(trieNode[trieIndex].right)
    {
      printTrie(trieNode[trieIndex].right, depth);
    }
  }

  void printTrieAnother()
  {
    for(int i=1;i<=trieNodeNumber;++i)
    {
      printf("%2d: %c child %2d right %2d ", i, trieNode[i].character>0?trieNode[i].character:' ', trieNode[i].child, trieNode[i].right);
      if(trieNode[i].word)
      {
        printf("WORD");
      }
      printf("\n");
    }
  }
#endif

void findWordCorrectNumber()
{
  for(int i=0;i<encryptedWordNumber;++i)
  {
    correctNumber[i] = 0;
    for(int j=0;j<encryptedWordLength[i];++j)
    {
      if(key[encryptedWord[i][j]])
      {
        ++correctNumber[i];
      }
    }
  }
}

int subSearch(int matchedWordNumber, int wordIndex, int wordPosition, int trieNumber);

int search(int matchedWordNumber)
{
  if(matchedWordNumber == encryptedWordNumber)
  {
    if(findAnswer)
    {
      return 2;
    }
    findAnswer = true;
    for(int i='A';i<='Z';++i)
    {
      answer[key[i]] = i;
    }
    return 1;
  }
  findWordCorrectNumber();
  int maxCorrect = -1;
  int wordIndex;
  for(int i=0;i<encryptedWordNumber;++i)
  {
    if(!matched[i])
    {
      if(correctNumber[i] > maxCorrect)
      {
        maxCorrect = correctNumber[i];
        wordIndex = i;
      }
    }
  }
  #if defined(TRIE1)
    return subSearch(matchedWordNumber, wordIndex, 0, 0);
  #elif defined(TRIE2)
    return subSearch(matchedWordNumber, wordIndex, 0, 1);
  #endif
}

int subSearch(int matchedWordNumber, int wordIndex, int wordPosition, int trieIndex)
{
  if(wordPosition == encryptedWordLength[wordIndex])
  {
    matched[wordIndex] = true;
    int temp = search(matchedWordNumber + 1);
    matched[wordIndex] = false;
    return temp;
  }
  if(!trieIndex)
  {
    return 0;
  }
  char currentCharacter = encryptedWord[wordIndex][wordPosition];
  if(key[currentCharacter])
  {
    #if defined(TRIE1)
      if(trieNode[trieIndex].next[key[currentCharacter] - 'A'])
      {
        if(wordPosition + 1 == encryptedWordLength[wordIndex] && !trieNode[trieNode[trieIndex].next[key[currentCharacter] - 'A']].word)
        {
          return 0;
        }
        return subSearch(matchedWordNumber, wordIndex, wordPosition + 1, trieNode[trieIndex].next[key[currentCharacter] - 'A']);
      }
      return 0;
    #elif defined(TRIE2)
      for(;trieIndex;trieIndex=trieNode[trieIndex].right)
      {
        if(trieNode[trieIndex].character == key[currentCharacter])
        {
          if(wordPosition + 1 == encryptedWordLength[wordIndex])
          {
            if(!trieNode[trieIndex].word)
            {
              return 0;
            }
          }
          return subSearch(matchedWordNumber, wordIndex, wordPosition + 1, trieNode[trieIndex].child);
        }
      }
      return 0;
    #endif
  }
  else
  {
    int find = 0;
    #if defined(TRIE1)
      for(int i=0;i<26;++i)
      {
        if(!used[i + 'A'] && trieNode[trieIndex].next[i])
        {
          if(wordPosition + 1 == encryptedWordLength[wordIndex] && !trieNode[trieNode[trieIndex].next[i]].word)
          {
            continue;
          }
          key[currentCharacter] = i + 'A';
          used[i + 'A'] = true;
          int temp = subSearch(matchedWordNumber, wordIndex, wordPosition + 1, trieNode[trieIndex].next[i]);
          used[i + 'A'] = false;
          key[currentCharacter] = 0;
          if(temp == 2)
          {
            return temp;
          }
          else if(temp == 1)
          {
            find = 1;
          }
        }
      }
    #elif defined(TRIE2)
      for(;trieIndex;trieIndex=trieNode[trieIndex].right)
      {
        if(!used[trieNode[trieIndex].character])
        {
          if(wordPosition + 1 == encryptedWordLength[wordIndex])
          {
            if(!trieNode[trieIndex].word)
            {
              continue;
            }
          }
          used[trieNode[trieIndex].character] = true;
          key[currentCharacter] = trieNode[trieIndex].character;
          int temp = subSearch(matchedWordNumber, wordIndex, wordPosition + 1, trieNode[trieIndex].child);
          key[currentCharacter] = 0;
          used[trieNode[trieIndex].character] = false;
          if(temp == 2)
          {
            return temp;
          }
          else if(temp == 1)
          {
            find = 1;
          }
        }
      }
    #endif
    return find;
  }
}

int main(int argc, char *argv[])
{
  //freopen("in.txt", "r", stdin);
  //freopen("out.txt", "w", stdout);
  char word[128];
  scanf("%d",&wordNumber);
  for(int i=0;i<wordNumber;++i)
  {
    scanf("%s", word);
    #if defined(TRIE1)
      addWordToTrie(trieNode[0], word);
    #elif defined(TRIE2)
      addWordToTrie(1, word);
    #endif
  }
  scanf("%d",&encryptedNumber);
  #if defined(INPUT_METHOD2)
    gets(word);
    gets(word);
  #endif
  for(int i=0;i<encryptedNumber;++i)
  {
    findAnswer = false;
    memset(key, 0, sizeof(key));
    memset(used, false, sizeof(used));
    memset(answer, 0, sizeof(answer));
    memset(matched, false, sizeof(matched));
    memset(encryptedWordLength, 0, sizeof(encryptedWordLength));
    encryptedWordNumber = 0;
    #if defined(INPUT_METHOD1)
      bool lastIsNewLine = false;
      int wordPosition = 0;
      char tempCharacter;
      while(scanf("%c",&tempCharacter), tempCharacter < 'A' || tempCharacter > 'Z');
      while(true)
      {
        if(tempCharacter >= 'A' && tempCharacter <= 'Z')
        {
          lastIsNewLine = false;
          encryptedWord[encryptedWordNumber][wordPosition++] = tempCharacter;
        }
        else if(tempCharacter == ' ')
        {
          lastIsNewLine = false;
          if(wordPosition)
          {
            encryptedWordLength[encryptedWordNumber] = wordPosition;
            encryptedWord[encryptedWordNumber][wordPosition] = 0;
            encryptedWordNumber ++;
            wordPosition = 0;
          }
        }
        else if(tempCharacter == '\n')
        {
          if(wordPosition)
          {
            encryptedWordLength[encryptedWordNumber] = wordPosition;
            encryptedWord[encryptedWordNumber][wordPosition] = 0;
            encryptedWordNumber ++;
            wordPosition = 0;
          }
          if(lastIsNewLine)
          {
            break;
          }
          else
          {
            lastIsNewLine = true;
          }
        }
        if(scanf("%c",&tempCharacter) == EOF)
        {
          if(wordPosition)
          {
            encryptedWordLength[encryptedWordNumber] = wordPosition;
            encryptedWord[encryptedWordNumber][wordPosition] = 0;
            encryptedWordNumber ++;
            break;
          }
        }
      }
    #elif defined(INPUT_METHOD2)
      char buffer[100];
      int length;
      while(true)
      {
        if(gets(buffer) == NULL)
        {
          break;
        }
        length = strlen(buffer);
        if(length == 0)
        {
          break;
        }
        int index = 0;
        while(true)
        {
          while(index < length && (buffer[index] < 'A' || buffer[index] > 'Z'))
          {
            index++;
          }
          if (index >= length)
          {
            break;
          }
          while (index < length && buffer[index] >= 'A' && buffer[index] <= 'Z')
          {
            encryptedWord[encryptedWordNumber][encryptedWordLength[encryptedWordNumber]++] = buffer[index];
            index++;
          }
          encryptedWord[encryptedWordNumber][encryptedWordLength[encryptedWordNumber]] = 0;
          encryptedWordNumber++;
        }
      }
    #endif
    int temp = search(0);
    if(temp == 0)
    {
      printf("#No solution#\n");
    }
    else if(temp == 1)
    {
      for(int i='A';i<='Z';++i)
      {
        if(answer[i])
        {
          printf("%c", answer[i]);
        }
        else
        {
          printf("*");
        }
      }
      printf("\n");
    }
    else
    {
      printf("#More than one solution#\n");
    }
  }
  return 0;
}
