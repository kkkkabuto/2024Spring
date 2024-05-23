<?php
#error_reporting(0);
class HelloPhp
{
    public $a;
    public $b;
    public function __construct(){
        $this->a = "phpinfo()";
        $this->b = "assert";
    }
    public function __destruct(){
        $a = $this->a;
        $b = $this->b;
    }
}
$cc=new HelloPhp("phpinfo()","assert");
echo(serialize($cc));
// O:8:"HelloPhp":2:{s:1:"a";s:9:"phpinfo()";s:1:"b";s:6:"assert";}