<?php
// p2 -  タイトルページ

include_once './conf/conf.inc.php';   // 基本設定ファイル読込
require_once './p2util.class.php';	// p2用のユーティリティクラス
require_once './filectl.class.php';

authorize(); //ユーザ認証

//=========================================================
// 変数
//=========================================================

if (!empty($GLOBALS['pref_dir_realpath_failed_msg'])) {
	$_info_msg_ht .= '<p>'.$GLOBALS['pref_dir_realpath_failed_msg'].'</p>';
}

$p2web_url_r = P2Util::throughIme($_conf['p2web_url']);

// パーミッション注意喚起 ================
if ($_conf['pref_dir'] == $datdir) {
	P2Util::checkDirWritable($_conf['pref_dir']);
} else {
	P2Util::checkDirWritable($_conf['pref_dir']);
	P2Util::checkDirWritable($datdir);
}

//=========================================================
// 前処理
//=========================================================
// ●ID 2ch オートログイン
if ($array = P2Util::readIdPw2ch()) {
	list($login2chID, $login2chPW, $autoLogin2ch) = $array;
	if ($autoLogin2ch) {
		include_once './login2ch.inc.php';
		login2ch();
	}
}

//=========================================================
// プリント設定
//=========================================================
$p_htm = array();

// 最新版チェック
if ($_conf['updatan_haahaa']) {
	$newversion_found = checkUpdatan();
}

// 認証ユーザ情報
$autho_user_ht = "";
if ($login['use']) {
	$autho_user_ht = "<p>ログインユーザ: {$login['user']} - ".date("Y/m/d (D) G:i")."</p>\n";
}

// 前回のログイン情報
if ($_conf['login_log_rec'] && $_conf['last_login_log_show']) {
	if (($log = P2Util::getLastAccessLog($_conf['login_log_file'])) !== false) {
		$p_htm['log'] = array_map('htmlspecialchars', $log);
		$p_htm['last_login'] =<<<EOP
前回のログイン情報 - {$p_htm['log']['date']}<br>
ユーザ: {$p_htm['log']['user']}<br>
IP: {$p_htm['log']['ip']}<br>
HOST: {$p_htm['log']['host']}<br>
UA: {$p_htm['log']['ua']}<br>
REFERER: {$p_htm['log']['referer']}
EOP;
	}
/*
	$p_htm['log'] =<<<EOP
<table cellspacing="0" cellpadding="2";>
	<tr>
		<td colspan="2">前回のログイン情報</td>
	</tr>
	<tr>
		<td align="right">時刻: </td><td>{$alog['date']}</td>
	</tr>
	<tr>
		<td align="right">ユーザ: </td><td>{$alog['user']}</td>
	</tr>
	<tr>
		<td align="right">IP: </td><td>{$alog['ip']}</td>
	</tr>
	<tr>
		<td align="right">HOST: </td><td>{$alog['host']}</td>
	</tr>
	<tr>
		<td align="right">UA: </td><td>{$alog['ua']}</td>
	</tr>
	<tr>
		<td align="right">REFERER: </td><td>{$alog['referer']}</td>
</table>
EOP;
*/
}

//=========================================================
// HTMLプリント
//=========================================================
$ptitle = "p2 - title";

P2Util::header_content_type();
if ($_conf['doctype']) { echo $_conf['doctype']; }
echo <<<EOP
<html lang="ja">
<head>
	<meta name="ROBOTS" content="NOINDEX, NOFOLLOW">
	<meta http-equiv="Content-Style-Type" content="text/css">
	<meta http-equiv="Content-Script-Type" content="text/javascript">
	<title>{$ptitle}</title>
	<base target="read">
EOP;

@include("./style/style_css.inc");

echo <<<EOP
</head>
<body>
EOP;

echo $_info_msg_ht;
$_info_msg_ht = "";

echo <<<EOP
<br>
<div class="container">
	{$newversion_found}
	<p>p2 version {$_conf['p2version']} 　<a href="{$p2web_url_r}" target="_blank">{$_conf['p2web_url']}</a></p>
	<ul>
		<li><a href="viewtxt.php?file=doc/README.txt">README.txt</a></li>
		<li><a href="img/how_to_use.png">ごく簡単な操作法</a></li>
		<li><a href="viewtxt.php?file=doc/ChangeLog.txt">ChangeLog（更新記録）</a></li>
	</ul>
	<!-- <p><a href="{$p2web_url_r}" target="_blank">p2 web &lt;{$_conf['p2web_url']}&gt;</a></p> -->
	{$autho_user_ht}
	{$p_htm['last_login']}
</div>
</body>
</html>
EOP;

//==================================================
// ■関数
//==================================================
/**
* オンライン上のp2最新版をチェックする
*/
function checkUpdatan()
{
	global $_conf, $p2web_url_r;

	$ver_txt_url = $_conf['p2web_url'] . 'p2status.txt';
	$cachefile = $_conf['pref_dir'] . '/p2_cache/p2status.txt';
	FileCtl::mkdir_for($cachefile);
	
	if (file_exists($cachefile)) {
		// キャッシュの更新が指定時間以内なら
		if (@filemtime($cachefile) > time() - $_conf['p2status_dl_interval'] * 60) {
			$no_p2status_dl_flag = true;
		}
	}
	
	if (!$no_p2status_dl_flag) {
		P2Util::fileDownload($ver_txt_url, $cachefile);
	}
	
	$ver_txt = file($cachefile);
	$update_ver = $ver_txt[0];
	$kita = 'ｷﾀ━━━━（ﾟ∀ﾟ）━━━━!!!!!!';
	//$kita = 'ｷﾀ*･ﾟﾟ･*:.｡..｡.:*･ﾟ(ﾟ∀ﾟ)ﾟ･*:.｡. .｡.:*･ﾟﾟ･*!!!!!';
	
	if ($update_ver && version_compare($update_ver, $_conf['p2version'], '>')) {
		$newversion_found = <<<EOP
<div class="kakomi">
	{$kita}<br>
	オンライン上に p2 の最新バージョンを見つけますた。<br>
	p2<!-- version {$update_ver}--> → <a href="{$p2web_url_r}cgi/dl/dl.php?dl=p2">ダウンロード</a> / <a href="{$p2web_url_r}p2/doc/ChangeLog.txt"{$_conf['ext_win_target_at']}>更新記録</a>
</div>
<hr class="invisible">
EOP;
	}
	return $newversion_found;
}

?>