<?php
// p2 - StrCtl -- �����񑀍�N���X

include_once("jcode.phps");

class StrCtl{

	function p2SJIStoEUC($str){
		if(extension_loaded('mbstring')){
			$str = mb_convert_encoding($str, "EUC-JP", "SJIS");
		}else{
			$str = JcodeConvert($str, 2, 1);
		}
		return $str;
	}
	
	function p2EUCtoSJIS($str){
		if(extension_loaded('mbstring')){
			$str = mb_convert_encoding($str, "SJIS", "EUC-JP");
		}else{
			$str = JcodeConvert($str, 1, 2);
		}
		return $str;
	}
	
	//============================================================
	// wordForMatch -- �t�H�[�����瑗���Ă������[�h���}�b�`�֐��ɓK�������郁�\�b�h
	//============================================================
	function wordForMatch($word){
		$word_fm = trim($word);
		//$word = str_replace("�@","|",$word);
		
		$word_fm = StrCtl::p2SJIStoEUC($word_fm);
		
		$word_fm = str_replace("/","\/",$word_fm);
		$word_fm = htmlspecialchars($word_fm, ENT_NOQUOTES);
		//$word_euc = str_replace(" ","|",$word_euc); //��ʓI�Ȍ����w��ɍ��킹��
		return $word_fm;
	}

}

?>