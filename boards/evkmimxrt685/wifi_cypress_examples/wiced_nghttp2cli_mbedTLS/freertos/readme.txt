Overview
========

The lwip_nghttp2cli_mbedTLS demo application demonstrates an NGHTTP2 client set up on lwIP TCP/IP and the MbedTLS stack with
FreeRTOS. The board acts as an HTTP client and sends a request to the HTTPServer.

Demo requires DHCP and DNS servers.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT685 board
- Murata 1DX M.2 module
- Murata uSD M.2 Adapter
- Personal Computer

Board settings
==============


Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert the Ethernet Cable into the target board's RJ45 port and connect it to your PC network adapter.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the terminal will display the following:


Getting IP address from DHCP ...

 IPv4 Address     : 10.42.0.218
DHCP OKTLS init

  . Seeding the random number generator...
Connecting to nghttp2.org/443
  . Setting up the SSL/TLS structure...


SSL state connect : 0  ok


SSL state connect : 0
  . Performing the SSL/TLS handshake...
 ok
    [ Protocol is TLSv1.2 ]
    [ Ciphersuite is TLS-ECDHE-ECDSA-WITH-AES-256-GCM-SHA384 ]
    [ Application Layer Protocol is h2 ]
    [ Record expansion is 29 ]
  . Verifying peer X.509 certificate...
 Server Verification skipped
  . Peer certificate information    ...
      cert. version     : 3
      serial number     : 04:AC:3A:D5:71:91:1C:2C:79:4E:5D:FD:B1:63:BA:73:05:8C
      issuer name       : C=US, O=Let's Encrypt, CN=Let's Encrypt Authority X3
      subject name      : CN=nghttp2.org
      issued  on        : 2018-06-20 00:00:09
      expires on        : 2018-09-18 00:00:09
      signed using      : RSA with SHA-256
      EC key size       : 256 bits
      basic constraints : CA=false
      subject alt name  : nghttp2.org, www.nghttp2.org
      key usage         : Digital Signature
      ext key usage     : TLS Web Server Authentication, TLS Web Client Authentication

NGHTTP2 init
Writing HTTP request...
Request headers:
:method: GET
:scheme: https
:authority: nghttp2.org
:path: /

created new stream: 1
NGHTTP2 start
Response headers for stream ID=1:
:status: 200
date: Wed, 18 Jul 2018 17:32:02 GMT
content-type: text/html
last-modified: Tue, 08 May 2018 13:53:22 GMT
etag: "5af1abd2-19d8"
accept-ranges: bytes
content-length: 6616
x-backend-header-rtt: 0.002431
strict-transport-security: max-age=31536000
server: nghttpx
via: 2 nghttpx
x-frame-options: SAMEORIGIN
x-xss-protection: 1; mode=block
x-content-type-options: nosniff

All headers received


<!DOCTYPE html>
<!--[if IEMobile 7 ]><html class="no-js iem7"><![endif]-->
<!--[if lt IE 9]><html class="no-js lte-ie8"><![endif]-->
<!--[if (gt IE 8)|(gt IEMobile 7)|!(IEMobile)|!(IE)]><!--><html class="no-js" lang="en"><!--<![endif]-->
<head>
  <meta charset="utf-8">
  <title>Nghttp2: HTTP/2 C Library - nghttp2.org</title>
  <meta name="author" content="Tatsuhiro Tsujikawa">


  <meta name="description" content="Nghttp2: HTTP/2 C Library Feb 16th, 2015 11:16 pm nghttp2 is an implementation of
HTTP/2 and its header
compression algorithm HPACK in
C. The &hellip;">


  <!-- http://t.co/dKP3o1e -->
  <meta name="HandheldFriendly" content="True">
  <meta name="MobileOptimized" content="320">
  <meta name="viewport" content="width=device-width, initial-scale=1">


  <link rel="canonical" href="//nghttp2.org">
  <link href="/favicon.png" rel="icon">
  <link href="/stylesheets/screen.css" media="screen, projection" rel="stylesheet" type="text/css">
  <link href="/atom.xml" rel="alternate" title="nghttp2.org" type="application/atom+xml">
  <script src="/javascripts/modernizr-2.0.js"></script>
  <script src="//ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>
  <script>!window.jQuery && document.write(unescape('%3Cscript src="./javascripts/libs/jquery.min.js"%3E%3C/script%3E'))</script>
  <script src="/javascripts/octopress.js" type="text/javascript"></script>
  <!--Fonts from Google"s Web font directory at http://google.com/webfonts -->
<link href="//fonts.googleapis.com/css?family=PT+Serif:regular,italic,bold,bolditalic" rel="stylesheet" type="text/css">
<link href="//fonts.googleapis.com/css?family=PT+Sans:regular,italic,bold,bolditalic" rel="stylesheet" type="text/css">



</head>

<body   >
  <header role="banner"><hgroup>
  <h1><a href="/">nghttp2.org</a></h1>

    <h2>HTTP/2 C library and tools</h2>

</hgroup>

</header>
  <nav role="navigation"><ul class="subscription" data-subscription="rss">
  <li><a href="/atom.xml" rel="subscribe-rss" title="subscribe via RSS">RSS</a></li>

</ul>

<form action="https://www.google.com/search" method="get">
  <fieldset role="search">
    <input type="hidden" name="q" value="site://nghttp2.org" />
    <input class="search" type="text" name="q" results="0" placeholder="Search"/>
  </fieldset>
</form>

<ul class="main-navigation">
  <li><a href="/">Top</a></li>
  <li><a href="/blog/">Blog</a></li>
  <li><a href="/blog/archives">Archives</a></li>
  <li><a href="/documentation/">Documentation</a></li>
  <li><a href="/httpbin">Httpbin</a></li>
  <li><a href="https://github.com/nghttp2/nghttp2/releases">Releases</a></li>
  <li><a href="https://github.com/nghttp2/nghttp2">Source Code</a></li>
</ul>

</nav>
  <div id="main">
    <div id="content">
      <div>
<article role="article">

  <header>
    <h1 class="entry-title">Nghttp2: HTTP/2 C Library</h1>
    <p class="meta">




<time class='entry-date' datetime='2015-02-16T23:16:00+09:00'><span class='date'><span class='date-month'>Feb</span> <span class='date-day'>16</span><span class='date-suffix'>th</span>, <span class='date-year'>2015</span></span> <span class='time'>11:16 pm</span></time></p>
  </header>

  <p>nghttp2 is an implementation of
<a href="https://tools.ietf.org/html/rfc7540">HTTP/2</a> and its header
compression algorithm <a href="https://tools.ietf.org/html/rfc7541">HPACK</a> in
C.</p>

<p>The framing layer of HTTP/2 is implemented as a form of reusable C
library.  On top of that, we have implemented HTTP/2 <a href="/documentation/nghttp.1.html">client</a>, <a href="/documentation/nghttpd.1.html">server</a>
and <a href="/documentation/nghttpx.1.html">proxy</a>.
We have also developed <a href="/documentation/h2load.1.html">load test and benchmarking tool</a> for HTTP/2.</p>

<p>We have participated in httpbis working group since HTTP/2 draft-04,
which is the first implementation draft.  Since then we have updated
nghttp2 library constantly to latest specification and nghttp2 is now
one of the most mature <a href="https://github.com/http2/http2-spec/wiki/Implementations">HTTP/2 implementations</a>.</p>

<p>All C APIs are <a href="/documentation/apiref.html">fully documented</a>.</p>

<p>HTTP/2 utilizes header compression method called <a href="https://tools.ietf.org/html/rfc7541">HPACK</a>.  We offer
HPACK encoder and decoder are available as <a href="/documentation/tutorial-hpack.html">public API</a>.</p>

<p>nghttp2 library itself is a bit low-level.  The experimental <a href="/documentation/libnghttp2_asio.html">high level C++ API</a> is also available.</p>

<p>We have <a href="/documentation/python-apiref.html">Python binding</a> of this libary, but we have not covered
everything yet.</p>


    <footer>
      <p class="meta">






<time class='entry-date' datetime='2015-02-16T23:16:00+09:00'><span class='date'><span class='date-month'>Feb</span> <span class='date-day'>16</span><span class='date-suffix'>th</span>, <span class='date-year'>2015</span></span> <span class='time'>11:16 pm</span></time>

      </p>

        <div class="sharing">

  <a href="//twitter.com/share" class="twitter-share-button" data-url="//nghttp2.org/index.html" data-via="" data-counturl="//nghttp2.org/index.html" >Tweet</a>



</div>


    </footer>

</article>

</div>

<aside class="sidebar">

    <section>
  <h1>Recent Posts</h1>
  <ul id="recent_posts">

      <li class="post">
        <a href="/blog/2018/05/08/nghttp2-v1-32-0/">Nghttp2 v1.32.0</a>
      </li>

      <li class="post">
        <a href="/blog/2018/04/12/nghttp2-v1-31-1/">Nghttp2 v1.31.1</a>
      </li>

      <li class="post">
        <a href="/blog/2018/02/27/nghttp2-v1-31-0/">Nghttp2 v1.31.0</a>
      </li>

      <li class="post">
        <a href="/blog/2018/02/02/nghttp2-v1-30-0/">Nghttp2 v1.30.0</a>
      </li>

      <li class="post">
        <a href="/blog/2017/12/19/nghttp2-v1-29-0/">Nghttp2 v1.29.0</a>
      </li>

  </ul>
</section>






</aside>


    </div>
  </div>
  <footer role="contentinfo"><p>
  Copyright &copy; 2018 - Tatsuhiro Tsujikawa -
  <span class="credit">Powered by <a href="http://octopress.org">Octopress</a></span>
</p>

</footer>








  <script type="text/javascript">
    (function(){
      var twitterWidgets = document.createElement('script');
      twitterWidgets.type = 'text/javascript';
      twitterWidgets.async = true;
      twitterWidgets.src = '//platform.twitter.com/widgets.js';
      document.getElementsByTagName('head')[0].appendChild(twitterWidgets);
    })();
  </script>





</body>
</html>

closed stream 1 with error_code=0 - NO_ERROR
html,body,div,span,applet,object,iframe,h1,h2,h3,h4,h5,h6,p,blockquote,pre,a,abbr,acronym,address,big,cite,code,del,dfn,em,img,ins,kbd,q,s,samp,small,strike,strong,sub,sup,tt,var,b,u,i,center,dl,dt,dd,ol,ul,li,fieldset,form,label,legend,table,caption,tbody,tfoot,thead,tr,th,td,article,aside,canvas,details,embed,figure,figcaption,footer,header,hgroup,menu,nav,output,ruby,section,summary,time,mark,audio,video{margin:0;padding:0;border:0;font:inherit;font-size:100%;vertical-align:baseline}html{line-height:1}ol,ul{list-style:none}table{border-collapse:collapse;border-spacing:0}caption,th,td{text-align:left;font-weight:normal;vertical-align:middle}q,blockquote{quotes:none}q:before,q:after,blockquote:before,blockquote:after{content:"";content:none}a img{border:none}article,aside,details,figcaption,figure,footer,header,hgroup,menu,nav,section,summary{display:block}a{color:#2aa198}a:visited{color:#2aa198}a:focus{color:#15e9d9}a:hover{color:#15e9d9}a:active{color:#0fa398}aside.sidebar a{color:#073642}aside.sidebar a:focus{color:#15e9d9}aside.sidebar a:hover{color:#15e9d9}aside.sidebar a:active{color:#0fa398}a{-webkit-transition:color 0.3s;-moz-transition:color 0.3s;-o-transition:color 0.3s;transition:color 0.3s}html{background:#002b36 url('/images/line-tile.png?1413623488') top left}body>div{background:#f2f2f2 url('/images/noise.png?1413623488') top left;border-bottom:1px solid #7d8d93}body>div>div{background:#fefcf6 url('/images/noise.png?1413623488') top left;border-right:1px solid #e0e0e0}.heading,body>header h1,h1,h2,h3,h4,h5,h6{font-family:"PT Serif","Georgia","Helvetica Neue",Arial,sans-serif}.sans,body>header h2,article header p.meta,article>footer,#content .blog-index footer,html .gist .gist-file .gist-meta,#blog-archives a.category,#blog-archives time,aside.sidebar section,body>footer{font-family:"PT Sans","Helvetica Neue",Arial,sans-serif}.serif,body,#content .blog-index a[rel=full-article]{font-family:"PT Serif",Georgia,Times,"Times New Roman",serif}.mono,pre,code,tt,p code,li code{font-family:Menlo,Monaco,"Andale Mono","lucida console","Courier New",monospace}body>header h1{font-size:2.2em;font-family:"PT Serif","Georgia","Helvetica Neue",Arial,sans-serif;font-weight:normal;line-height:1.2em;margin-bottom:0.6667em}body>header h2{font-family:"PT Serif","Georgia","Helvetica Neue",Arial,sans-serif}body{line-height:1.5em;color:#073642}h1{font-size:2.2em;line-height:1.2em}@media only screen and (min-width: 992px){body{font-size:1.15em}h1{font-size:2.6em;line-height:1.2em}}h1,h2,h3,h4,h5,h6{text-rendering:optimizelegibility;margin-bottom:1em;font-weight:bold}h2,section h1{font-size:1.5em}h3,section h2,section section h1{font-size:1.3em}h4,section h3,section section h2,section section section h1{font-size:1em}h5,section h4,section section h3{font-size:.9em}h6,section h5,section section h4,section section section h3{font-size:.8em}p,article blockquote,ul,ol{margin-bottom:1.5em}ul{list-style-type:disc}ul ul{list-style-type:circle;margin-bottom:0px}ul ul ul{list-style-type:square;margin-bottom:0px}ol{list-style-type:decimal}ol ol{list-style-type:lower-alpha;margin-bottom:0px}ol ol ol{list-style-type:lower-roman;margin-bottom:0px}ul,ul ul,ul ol,ol,ol ul,ol ol{margin-left:1.3em}ul ul,ul ol,ol ul,ol ol{margin-bottom:0em}strong{font-weight:bold}em{font-style:italic}sup,sub{font-size:0.75em;position:relative;display:inline-block;padding:0 .2em;line-height:.8em}sup{top:-.5em}sub{bottom:-.5em}a[rev='footnote']{font-size:.75em;padding:0 .3em;line-height:1}q{font-style:italic}q:before{content:"\201C"}q:after{content:"\201D"}em,dfn{font-style:italic}strong,dfn{font-weight:bold}del,s{text-decoration:line-through}abbr,acronym{border-bottom:1px dotted;cursor:help}hr{margin-bottom:0.2em}small{font-size:.8em}big{font-size:1.2em}article blockquote{font-style:italic;position:relative;font-size:1.2em;line-height:1.5em;padding-left:1em;border-left:4px solid rgba(170,170,170,0.5)}article blockquote cite{font-style:italic}article blockquote cite a{color:#aaa !important;word-wrap:break-word}article blockquote cite:before{content:'\2014';padding-right:.3em;padding-left:.3em;color:#aaa}@media only screen and (min-width: 992px){article blockquote{padding-left:1.5em;border-left-width:4px}}.pullquote-right:before,.pullquote-left:before{padding:0;border:none;content:attr(data-pullquote);float:right;width:45%;margin:.5em 0 1em 1.5em;position:relative;top:7px;font-size:1.4em;line-height:1.45em}.pullquote-left:before{float:left;margin:.5em 1.5em 1em 0}.force-wrap,article a,aside.sidebar a{white-space:-moz-pre-wrap;white-space:-pre-wrap;white-space:-o-pre-wrap;white-space:pre-wrap;word-wrap:break-word}.group,body>header,body>nav,body>footer,body #content>article,body #content>div>article,body #content>div>section,body div.pagination,aside.sidebar,#main,#content,.sidebar{*zoom:1}.group:after,body>header:after,body>nav:after,body>footer:after,body #content>article:after,body #content>div>section:after,body div.pagination:after,#main:after,#content:after,.sidebar:after{content:"";display:table;clear:both}body{-webkit-text-size-adjust:none;max-width:1200px;position:relative;margin:0 auto}body>header,body>nav,body>footer,body #content>article,body #content>div>article,body #content>div>section{padding-left:18px;padding-right:18px}@media only screen and (min-width: 480px){body>header,body>nav,body>footer,body #content>article,body #content>div>article,body #content>div>section{padding-left:25px;padding-right:25px}}@media only screen and (min-width: 768px){body>header,body>nav,body>footer,body #content>article,body #content>div>article,body #content>div>section{padding-left:35px;padding-right:35px}}@media only screen and (min-width: 992px){body>header,body>nav,body>footer,body #content>article,body #content>div>article,body #content>div>section{padding-left:55px;padding-right:55px}}body div.pagination{margin-left:18px;margin-right:18px}@media only screen and (min-width: 480px){body div.pagination{margin-left:25px;margin-right:25px}}@media only screen and (min-width: 768px){body div.pagination{margin-left:35px;margin-right:35px}}@media only screen and (min-width: 992px){body div.pagination{margin-left:55px;margin-right:55px}}body>header{font-size:1em;padding-top:1.5em;padding-bottom:1.5em}#content{overflow:hidden}#content>div,#content>article{width:100%}aside.sidebar{float:none;padding:0 18px 1px;background-color:#f7f7f7;border-top:1px solid #e0e0e0}.flex-content,article img,article video,article .flash-video,aside.sidebar img{max-width:100%;height:auto}.basic-alignment.left,article img.left,article video.left,article .left.flash-video,aside.sidebar img.left{float:left;margin-right:1.5em}.basic-alignment.right,article img.right,article video.right,article .right.flash-video,aside.sidebar img.right{float:right;margin-left:1.5em}.basic-alignment.center,article img.center,article video.center,article .center.flash-video,aside.sidebar img.center{display:block;margin:0 auto 1.5em}.basic-alignment.left,article img.left,article video.left,article .left.flash-video,aside.sidebar img.left,.basic-alignment.right,article img.right,article video.right,article .right.flash-video,aside.sidebar img.right{margin-bottom:.8em}.toggle-sidebar,.no-sidebar .toggle-sidebar{display:none}@media only screen and (min-width: 750px){body.sidebar-footer aside.sidebar{float:none;width:auto;clear:left;margin:0;padding:0 35px 1px;background-color:#f7f7f7;border-top:1px solid #eaeaea}body.sidebar-footer aside.sidebar section.odd,body.sidebar-footer aside.sidebar section.even{float:left;width:48%}body.sidebar-footer aside.sidebar section.odd{margin-left:0}body.sidebar-footer aside.sidebar section.even{margin-left:4%}body.sidebar-footer aside.sidebar.thirds section{width:30%;margin-left:5%}body.sidebar-footer aside.sidebar.thirds section.first{margin-left:0;clear:both}}body.sidebar-footer #content{margin-right:0px}body.sidebar-footer .toggle-sidebar{display:none}@media only screen and (min-width: 550px){body>header{font-size:1em}}@media only screen and (min-width: 750px){aside.sidebar{float:none;width:auto;clear:left;margin:0;padding:0 35px 1px;background-color:#f7f7f7;border-top:1px solid #eaeaea}aside.sidebar section.odd,aside.sidebar section.even{float:left;width:48%}aside.sidebar section.odd{margin-left:0}aside.sidebar section.even{margin-left:4%}aside.sidebar.thirds section{width:30%;margin-left:5%}aside.sidebar.thirds section.first{margin-left:0;clear:both}}@media only screen and (min-width: 768px){body{-webkit-text-size-adjust:auto}body>header{font-size:1.2em}#main{padding:0;margin:0 auto}#content{overflow:visible;margin-right:240px;position:relative}.no-sidebar #content{margin-right:0;border-right:0}.collapse-sidebar #content{margin-right:20px}#content>div,#content>article{padding-top:17.5px;padding-bottom:17.5px;float:
NGHTTP2 finished.
Customization options
=====================

