2 steps are involved. 

First, we need to bypass the login token check. The public key is already there and the validateToken function is vulnerbale. Use the Public key
to generate new token using the uid of admin

Use the token in exploit provided