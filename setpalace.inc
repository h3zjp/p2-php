<?php
// p2 -  �a������֌W�̏���

require_once("filectl_class.inc");
require_once("datactl.inc");

//==================================================================
// �ϐ�
//==================================================================

if($_GET['setpal']=="0" or $_GET['setpal']=="1"){
	$palbool=$_GET['setpal'];
}

//==================================================================
// key.idx�ɓǂݍ���
//==================================================================

//idxfile�̃p�X�����߂�
$datdir_host = datdirOfHost($_GET["host"]);
$idxfile = $datdir_host."/".$_GET['bbs']."/".$_GET['key'].".idx";

//�f�[�^������Ȃ�ǂݍ����
if( is_readable($idxfile) ){
	$lines=@file($idxfile);
	$data = explode("<>", $lines[0]);
}

//==================================================================
// p2_palace.idx�ɏ�������
//==================================================================
$palace_idx = $prefdir. '/p2_palace.idx';

//================================================
// �ǂݍ���
//================================================

// p2_palace �t�@�C�����Ȃ���ΐ���
FileCtl::make_datafile($palace_idx, $palace_perm);

//palace_idx�ǂݍ���;
$pallines= @file($palace_idx);

//================================================
// ����
//================================================

//�ŏ��ɏd���v�f���폜
if($pallines){
	$i=-1;
	unset($neolines);
	foreach($pallines as $pall){
		$i++;
		$lar = explode("<>", $pall);
		if($lar[1]==$_GET['key']){ //�d�����
			$before_line_num=$i;
			continue;
		}elseif(! $lar[1]){ //key�̂Ȃ����͕̂s���f�[�^
			continue;
		}else{
			$neolines[]=$pall;
		}
	}
}

//�V�K�f�[�^�ݒ�
if($_GET['setpal']){
	$newdata="$data[0]<>{$_GET['key']}<>$data[2]<>$data[3]<>$data[4]<>$data[5]<>$data[6]<>$data[7]<>$data[8]<>$data[9]<>{$_GET['host']}<>{$_GET['bbs']}\n";
}
	
if($_GET['setpal']==1 or $_GET['setpal']=="top"){
	$after_line_num=0;
	
}elseif($_GET['setpal']=="up"){
	$after_line_num=$before_line_num-1;
	if($after_line_num<0){$after_line_num=0;}
	
}elseif($_GET['setpal']=="down"){
	$after_line_num=$before_line_num+1;
	if( $after_line_num >= sizeof($neolines) ){$after_line_num="bottom";}
	
}elseif($_GET['setpal']=="bottom"){
	$after_line_num="bottom";
}

//================================================
//��������
//================================================
$fp = @fopen($palace_idx,"wb") or die("Error: $palace_idx ���X�V�ł��܂���ł���");
if($neolines){
	$i=0;
	foreach($neolines as $l){
		if($i===$after_line_num){fputs($fp, $newdata);}
		fputs($fp, $l);
		$i++;
	}
	if($after_line_num==="bottom"){fputs($fp, $newdata);}
	//�u$after_line_num=="bottom"�v���ƌ듮�삷��B
}else{
	fputs($fp, $newdata);
}
fclose($fp);

?>