extern volatile char key[128];

char keyStatus[128];

void refreshKeys()
{
    int i;
    
    for (i = 0; i < 128; i++)
        if (key[i])
        {
            if (keyStatus[i] < 2)
                keyStatus[i]++;
            else
                keyStatus[i] = 2;    
        }
        else
            keyStatus[i] = 0;
}
