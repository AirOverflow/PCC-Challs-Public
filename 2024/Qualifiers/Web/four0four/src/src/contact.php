<?php
// function hi($person) {
//     echo "Hello, i'll see you again $person! ^_^ \n";
// }

class Contact {
    public $p1 = 'pink';
    public $p2 = 'blue';
    public $p3 = 'red';

    function __toString() {
        return "This is a resource object\n";
    }

    function __destruct() {
        (
            $this->p1
        )(
            $this->p2,
            $this->p3
        );
    }

    function __invoke() {
        echo "This is a resource object\n";
    }

    function iDoNothing() {
        return;
    }
}
?>