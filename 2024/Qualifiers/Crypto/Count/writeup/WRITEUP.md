### Count

---


Cipher1 XOR Cipher2 = cipher3


now xor the cipher3 with known words, as we know the flag start with 'PCC{', we get the initial text


cipher3 XOR 'PCC{' = 'I do '
now we need to think what comes after 'I do' 
'I do what ...'
'I do not ... ' ? 

lets try 'I do not'

we get the readable flag, 

now we can google search as such 

google: "i do not" quote from leader

we get a full quote form Quaid-e-Azam

now again XOR with cipher3 we get the whole flag