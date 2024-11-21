The challenge can be solved by created a serialized phar file with PHP. Use it to include the `contact.php` code:

The `exploit.php` file is included. Use the below command to execute the php file:

```
php --define phar.readonly=0 .\exploit.php
```

Afterwards, rename the file from exploit.phar to exploit.gif to be upload it. Once, uploaded you shall receive the flag on API. 