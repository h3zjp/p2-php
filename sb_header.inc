<?php
// p2 -  �T�u�W�F�N�g - �w�b�_�\��
// for subject.php

//===================================================================
// �ϐ�
//===================================================================
$newtime= date("gis");
$reloaded_time = date("m/d G:i:s"); //�X�V����

// �X�����ځ[��`�F�b�N�A�q�� =============================================
if($aThreadList->spmode == "taborn" || $aThreadList->spmode == "soko" and $aThreadList->threads){
	$offline_num=$aThreadList->num - $online_num;
	$taborn_check_ht = <<<EOP
	<form class="check" method="POST" action="{$_SERVER['PHP_SELF']}" target="_self">\n
EOP;
	if($offline_num>0){
		if($aThreadList->spmode == "taborn"){
			$taborn_check_ht .= <<<EOP
		<p>{$aThreadList->num}�����A{$offline_num}���̃X���b�h�����ɔT�[�o�̃X���b�h�ꗗ����O��Ă���悤�ł��i�����Ń`�F�b�N�����܂��j</p>\n
EOP;
		}
		/*
		elseif($aThreadList->spmode == "soko"){
			$taborn_check_ht .= <<<EOP
		<p>{$aThreadList->num}����dat�����X���b�h���ۊǂ���Ă��܂��B</p>\n
EOP;
		}*/
	}
}

//===============================================================
// HTML�\���p�ϐ� for �c�[���o�[(sb_toolbar.inc) 
//===============================================================

$norefresh_q="&amp;norefresh=true";

// �y�[�W�^�C�g������URL�ݒ� ====================================
if($aThreadList->spmode=="taborn" or $aThreadList->spmode=="soko"){
	$ptitle_url = "{$subject_php}?host={$aThreadList->host}&amp;bbs={$aThreadList->bbs}";
}elseif($aThreadList->spmode=="res_hist"){
	$ptitle_url = "./read_res_hist.php#footer";
}elseif(! $aThreadList->spmode){
	$ptitle_url = "http://{$aThreadList->host}/{$aThreadList->bbs}/";
	if( preg_match("/www\.onpuch\.jp/", $aThreadList->host) ){$ptitle_url = $ptitle_url."index2.html";}
	if( preg_match("/livesoccer\.net/", $aThreadList->host) ){$ptitle_url = $ptitle_url."index2.html";}
	//match�o�^���head�Ȃ��ĕ������ق����悳���������A�������X�|���X������̂�����
}

// �y�[�W�^�C�g������HTML�ݒ� ====================================
if($aThreadList->spmode == "taborn"){
	$ptitle_ht = <<<EOP
	<span class="itatitle"><a class="aitatitle" href="{$ptitle_url}" target="_self"><b>{$aThreadList->itaj}</b></a>�i���ځ[�񒆁j</span>
EOP;
}elseif($aThreadList->spmode == "soko"){
	$ptitle_ht = <<<EOP
	<span class="itatitle"><a class="aitatitle" href="{$ptitle_url}" target="_self"><b>{$aThreadList->itaj}</b></a>�idat�q�Ɂj</span>
EOP;
}elseif($ptitle_url){
	$ptitle_ht = <<<EOP
	<span class="itatitle"><a class="aitatitle" href="{$ptitle_url}"><b>{$aThreadList->ptitle}</b></a></span>
EOP;
}else{
	$ptitle_ht = <<<EOP
	<span class="itatitle"><b>{$aThreadList->ptitle}</b></span>
EOP;
}

//�r���[�����ݒ� ==============================================
if($aThreadList->spmode){ //�X�y�V�������[�h��
	if($aThreadList->spmode=="fav" or $aThreadList->spmode=="palace"){//���C�ɃX�� or �a���Ȃ�
		if($sb_view=="edit"){
			$edit_ht="<a class=\"narabi\" href=\"{$subject_php}?spmode={$aThreadList->spmode}{$norefresh_q}\" target=\"_self\">����</a>";
		}else{
			$edit_ht="<a class=\"narabi\" href=\"{$subject_php}?spmode={$aThreadList->spmode}&amp;sb_view=edit{$norefresh_q}\" target=\"_self\">����</a>";

		}
	}
}

//�t�H�[��hidden==================================================
if($aThreadList->bbs){
	$input_bbs_ht="<input type=\"hidden\" name=\"bbs\" value=\"{$aThreadList->bbs}\">";
}
if($aThreadList->host){
	$input_host_ht="<input type=\"hidden\" name=\"host\" value=\"{$aThreadList->host}\">";
}
if($aThreadList->spmode){
	$input_spmode_ht="<input type=\"hidden\" name=\"spmode\" value=\"{$aThreadList->spmode}\">";
}

$sb_form_hidden_ht =<<<EOP
		$input_bbs_ht
		$input_host_ht
		$input_spmode_ht
EOP;

//�\������ ==================================================
if(!$aThreadList->spmode || $aThreadList->spmode=="news"){

	if($p2_setting['viewnum']=="100"){$vncheck_100=" selected";}
	elseif($p2_setting['viewnum']=="150"){$vncheck_150=" selected";}
	elseif($p2_setting['viewnum']=="200"){$vncheck_200=" selected";}
	elseif($p2_setting['viewnum']=="250"){$vncheck_250=" selected";}
	elseif($p2_setting['viewnum']=="300"){$vncheck_300=" selected";}
	elseif($p2_setting['viewnum']=="400"){$vncheck_400=" selected";}
	elseif($p2_setting['viewnum']=="500"){$vncheck_500=" selected";}
	elseif($p2_setting['viewnum']=="all"){$vncheck_all=" selected";}
	else{$p2_setting['viewnum']="150"; $vncheck_150=" selected";} //��{�ݒ�
	
	$sb_disp_num_ht =<<<EOP
		<select name="viewnum">
			<option value="100"{$vncheck_100}>100��</option>
			<option value="150"{$vncheck_150}>150��</option>
			<option value="200"{$vncheck_200}>200��</option>
			<option value="250"{$vncheck_250}>250��</option>
			<option value="300"{$vncheck_300}>300��</option>
			<option value="400"{$vncheck_400}>400��</option>
			<option value="500"{$vncheck_500}>500��</option>
			<option value="all"{$vncheck_all}>�S��</option>
		</select>
EOP;
}

//�t�B���^���� ==================================================
$filter_form_ht =<<<EOP
		<form class="toolbar" method="GET" action="subject.php" target="_self">
		$sb_form_hidden_ht
			<input type="text" id="word" name="word" value="{$word}" size="16">
			<input type="submit" name="submit_kensaku" value="����">
		</form>
EOP;



// �`�F�b�N�t�H�[�� =====================================
if($aThreadList->spmode == "taborn"){
	$abornoff_ht=<<<EOP
	<input type="submit" name="submit" value="{$abornoff_st}">
EOP;
}
if($aThreadList->spmode == "taborn" || $aThreadList->spmode == "soko" and $aThreadList->threads){
	$check_form_ht=<<<EOP
	<p>
		�`�F�b�N�������ڂ�
		<input type="submit" name="submit" value="{$deletelog_st}">
		$abornoff_ht
	</p>
EOP;
}

//===================================================================
// HTML�v�����g
//===================================================================

header_content_type();
if($doctype){ echo $doctype;}
echo <<<EOP
<html lang="ja">
<head>
	<meta name="ROBOTS" content="NOINDEX, NOFOLLOW">
	<meta http-equiv="Content-Style-Type" content="text/css">
	<meta http-equiv="Content-Script-Type" content="text/javascript">\n
EOP;

if($refresh_time){
	$refresh_time_s = $refresh_time*60;
	$refresh_url="{$subject_php}?host={$aThreadList->host}&amp;bbs={$aThreadList->bbs}&amp;spmode={$aThreadList->spmode}";
	echo <<<EOP
	<meta http-equiv="refresh" content="{$refresh_time_s};URL={$refresh_url}">
EOP;
}

echo <<<EOP
	<title>{$aThreadList->ptitle}</title>
	<base target="read">
EOP;

@include("./style/style_css.inc"); //��{�X�^�C���V�[�g�Ǎ�
@include("./style/subject_css.inc"); //subject�p�X�^�C���V�[�g�Ǎ�

echo <<<EOJS
	<script type="text/javascript" src="{$basic_js}"></script>
	<script language="JavaScript">
	<!--
	function setWinTitle(){
		var shinchaku_ari = "$shinchaku_attayo";
		if(shinchaku_ari){
			window.top.document.title="��{$aThreadList->ptitle}";
		}else{
			if (top != self) {top.document.title=self.document.title;}
		}
	}

	function chNewAllColor()
	{
		var smynum1 = document.getElementById('smynum1');
		if (smynum1) {
			smynum1.style.color="{$STYLE['sb_ttcolor']}";
		}
		var smynum2 = document.getElementById('smynum2')
		if (smynum2) {
			smynum2.style.color="{$STYLE['sb_ttcolor']}";
		}
		var a = document.getElementsByTagName('a');
		for (var i = 0; i < a.length; i++) {
			if (a[i].className == 'un_a') {
				a[i].style.color = "{$STYLE['sb_ttcolor']}";
			}
		}
	}
	
	function chUnColor(idnum){
		unid='un'+idnum;
		document.getElementById(unid).style.color="{$STYLE['sb_ttcolor']}";
	}
	function chTtColor(idnum){
		ttid="tt"+idnum;
		toid="to"+idnum;
		document.getElementById(ttid).style.color="{$STYLE['thre_title_color_v']}";
		document.getElementById(toid).style.color="{$STYLE['thre_title_color_v']}";
	}
	// -->
	</script>
EOJS;

if($aThreadList->spmode=="taborn" or $aThreadList->spmode=="soko")
	echo <<<EOJS
	<script language="javascript">
	<!--
	function checkAll(){
		var trk = 0;
		var inp = document.getElementsByTagName('input');
		for (var i=0; i<inp.length; i++){
			var e = inp[i];
			if ((e.name != 'allbox') && (e.type=='checkbox')){
				trk++;
				e.checked = document.getElementById('allbox').checked;
			}
		}
	}
	// -->
	</script>
EOJS;

echo <<<EOP
</head>
<body leftmargin="0" topmargin="0" marginwidth="0" marginheight="0" onLoad="setWinTitle();">
EOP;

include("./sb_toolbar.inc");

echo $_info_msg_ht;
$_info_msg_ht="";

echo <<<EOP
	$taborn_check_ht
	$check_form_ht
	<table cellspacing="0" width="100%">\n
EOP;

?>