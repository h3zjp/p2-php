<?php

require_once("./conf.php");  //��{�ݒ�
require_once("./filectl_class.inc");
require_once("./datactl.inc");
require_once("./crypt_xor.inc");

/**
 * ��2ch�Ƀ��O�C������֐�
 */
function login2ch()
{
	global $_conf, $prefdir, $idpw2ch_php, $sid2ch_php, $proxy;
	global $_info_msg_ht, $pass_perm, $crypt_xor_key;

	$php_curl_done_flag = false;
	
	// ��2ch ID&PW �ǂݍ���
	if (file_exists($idpw2ch_php)) {
		include($idpw2ch_php);
		$login2chPW = base64_decode($login2chPW);
		$login2chPW = decrypt_xor($login2chPW, $crypt_xor_key);
	} else {
		$_info_msg_ht .= "<p>p2 Error: ���O�C���̂��߂�ID�ƃp�X���[�h��o�^���ĉ������B[<a href=\"login2ch.php\" target=\"subject\">2ch���O�C���Ǘ�</a>]</p>";
		return false;
	}

	$auth2ch_url = "https://2chv.tora3.net/futen.cgi";
	$postf = "ID=".$login2chID."&PW=".$login2chPW;
	$x_2ch_ua = "X-2ch-UA: ".$_conf['p2name']."/".$_conf['p2version'];
	$dolib2ch = "DOLIB/1.00";
	$tempfile = $prefdir."/p2temp.php";
	
	// �O�̂��߂��炩����temp�t�@�C�����������Ă���
	if( file_exists($tempfile) ){ unlink($tempfile); }
	

	// ��system�R�}���h��curl�����s
	$curlrtn=1;
	// proxy�̐ݒ�
	if($proxy['use']){
		$with_proxy = " -x ".$proxy['host'].":".$proxy['port'];
	}else{
		$with_proxy = "";
	}
	// ���usystem�R�}���h��curl�v�i�ؖ������؂���j�����s
	$curlcmd = "curl -H \"{$x_2ch_ua}\" -A {$dolib2ch} -d ID={$login2chID} -d PW={$login2chPW} -o {$tempfile}{$with_proxy} {$auth2ch_url}";
	system($curlcmd, $curlrtn);
	
	// ���usystem�R�}���h��curl�v�i�ؖ������؂���j�Ŗ����������Ȃ�A�i�ؖ������؂Ȃ��j�ōă`�������W
	if($curlrtn!=0){
		$curlcmd = "curl -H \"{$x_2ch_ua}\" -A {$dolib2ch} -d ID={$login2chID} -d PW={$login2chPW} -o {$tempfile}{$with_proxy} -k {$auth2ch_url}";
		system($curlcmd, $curlrtn);
	}
		
	// ��system�R�}���h��curl�Ŗ����������Ȃ�
	if($curlrtn!=0){
	
		// ��PHP��CURL���g����Ȃ�A����Ń`�������W
		if( extension_loaded('curl') ){
			// ���uPHP��curl�v�i�ؖ������؂���j�Ŏ��s
			getAuth2chWithPhpCurl($tempfile, $auth2ch_url, $x_2ch_ua, $dolib2ch, $postf, true);
			// ���uPHP��curl�v�i�ؖ������؂���j�Ŗ����Ȃ�A�uPHP��curl�v�i�ؖ������؂Ȃ��j�ōă`�������W
			if(! file_exists($tempfile) ){
				getAuth2chWithPhpCurl($tempfile, $auth2ch_url, $x_2ch_ua, $dolib2ch, $postf, false);
			}
			$php_curl_done_flag = true;	// PHP��CURL�Ŏ��s������A�Ƃ����t���O
			
		}else{
			$_info_msg_ht .= "<p>p2 info: 2�����˂�ւ́�ID���O�C�����s���ɂ́Asystem��curl�R�}���h���g�p�\�ł��邩�APHP��<a href=\"http://www.php.net/manual/ja/ref.curl.php\">CURL�֐�</a>���L���ł���K�v������܂��B</p>";
		}
	}

	
	/*
	��PHP �� fsockopen �� https �ɑΉ����Ă��Ȃ��̂Ŗ����Ȃ̂�
	$post=array("ID"=>$login2chID, "PW"=>$login2chPW);
	$headers=$x_2ch_ua."\r\n";
	echo $r=getHttpContents($auth2ch_url, "", "POST", $headers, $post, $dolib2ch);
	*/
	
	if(! $r = FileCtl::get_file_contents($tempfile) ){
		if( file_exists($sid2ch_php) ){ unlink($sid2ch_php); }
		if( $php_curl_done_flag ){
			$setsumei = "�i�usystem��curl�R�}���h�v�uPHP��curl�v�̗����Ƃ����s���s���܂����j";
		}else{
			$setsumei = "�i�usystem��curl�R�}���h�v�Ŏ��s���s�B�uPHP��curl�v�͎g���Ȃ��悤�ł��j";
		} 
		$_info_msg_ht .= "<p>p2 Error: 2ch���O�C�������Ɏ��s���܂����B{$setsumei}</p>";
		return false;
	}
	
	// temp�t�@�C���͂����Ɏ̂Ă�
	if( file_exists($tempfile) ){ unlink($tempfile); }
	
	$r=rtrim($r);
	
	// ����
	if( preg_match("/SESSION-ID=(.+?):(.+)/", $r, $matches) ){
		$uaMona = $matches[1];
		$SID2ch = $matches[1].":".$matches[2];
	}else{
		if( file_exists($sid2ch_php) ){ unlink($sid2ch_php); }
		$_info_msg_ht .= "<p>p2 Error: ���O�C���ڑ��Ɏ��s���܂����B</p>";
		return false;
	}
	
	if($uaMona=="ERROR"){
		if( file_exists($idpw2ch_php) ){ unlink($idpw2ch_php); }
		if( file_exists($sid2ch_php) ){ unlink($sid2ch_php); }
		$_info_msg_ht .= "<p>p2 Error: SESSION-ID�̎擾�Ɏ��s���܂����BID�ƃp�X���[�h���m�F�̏�A���O�C���������ĉ������B</p>";
		return false;
	}

	//echo $r;//
	
	// SID�̋L�^�ێ� =======================
	$cont=<<<EOP
<?php
\$uaMona='{$uaMona}';
\$SID2ch='{$SID2ch}';
?>
EOP;
	FileCtl::make_datafile($sid2ch_php, $pass_perm); //$sid2ch_php ���Ȃ���ΐ���
	$fp = @fopen($sid2ch_php,"w");
	if(!$fp){
		$_info_msg_ht .= "<p>p2 Error: {$sid2ch_php} ��ۑ��ł��܂���ł����B���O�C���o�^���s�B</p>";
		return false;
	}
	fwrite($fp,$cont);
	fclose($fp);

	return $SID2ch;
}

//===================================================
// ��PHP��curl��2ch���O�C����SID�𓾂�֐�
//===================================================
function getAuth2chWithPhpCurl($tempfile, $auth2ch_url, $x_2ch_ua, $dolib2ch, $postf, $withk=false){
	global $proxy;
	
	$ch = curl_init();
	$fp = fopen ($tempfile, "w");
	curl_setopt ($ch, CURLOPT_FILE, $fp);
	curl_setopt ($ch, CURLOPT_URL, $auth2ch_url);
	curl_setopt ($ch, CURLOPT_HEADER, 0);
	curl_setopt ($ch, CURLOPT_HTTPHEADER, array($x_2ch_ua));
	curl_setopt ($ch, CURLOPT_USERAGENT, $dolib2ch);
	curl_setopt ($ch, CURLOPT_POST, 1);
	curl_setopt ($ch, CURLOPT_POSTFIELDS, $postf);
	if($withk){	// �ؖ����̌��؂����Ȃ��Ȃ�
		curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, 0);
	}
	if($proxy['use']){
		curl_setopt ($ch, CURLOPT_PROXY, $proxy['host'].':'.$proxy['port']);
	}
	curl_exec ($ch);
	curl_close ($ch);
	fclose ($fp);
}

?>