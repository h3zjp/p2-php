<?php
// p2 �[�X�^�C���ݒ�
// for read.php

echo <<<EOP
<style type="text/css" media="all">
<!--
EOP;

if($STYLE['fontfamily_bold']){
	echo <<<EOP
	h3{font-weight:normal; font-family:"{$STYLE['fontfamily_bold']}";} /* �X���b�h�^�C�g��*/
EOP;
}

echo <<<EOP
	body{
		background:{$STYLE['read_bgcolor']};
		line-height:130%;
		color:{$STYLE['read_color']};
		/*font-family:"���i�[";*/
	}
	body, td{
		font-size:{$STYLE['read_fontsize']};
	}
	
	a:link{color:{$STYLE['read_acolor']};}
	a:visited{color:{$STYLE['read_acolor_v']};}
	a:hover{color:{$STYLE['read_acolor_h']};}
	
	i{font-style:normal;} /* ���p���X*/
	dd.respopup{margin:8px 8px;} /* ���X�|�b�v�A�b�v*/
	
	.thread_title{margin:6px 0; line-height:120%; font-size:14pt; color:{$STYLE['read_thread_title_color']};}
	.thre_title{color:{$STYLE['read_thread_title_color']};}
	.name{color:{$STYLE['read_name_color']};} /* ���e�҂̖��O */
	.mail{color:{$STYLE['read_mail_color']};} /* ���e�҂�mail */
	.sage{color:{$STYLE['read_mail_sage_color']};} /* ���e�҂�mail(sage) */
	img.thumbnail{border:solid 1px;} /* �摜URL�̐�ǂ݃T���l�C��*/
	
	/* �V�����X�ԍ��i�����ł̓J���[���V���m�F�̋@�\�������Ă���̂œ��ʂ�font��
	�J���[�w������Ă���Bthread_class.inc - transRes ���Q��)	*/
	/* .newres{color:{$newres_color};} �� ����Č��݂͖����̐ݒ� */

	.onthefly{ /* on the fly */
		color:#0a0;
		border:1px #0a0 solid;
		padding:2px;
		font-size:11px;
	}
	.ontheflyresorder{
		color:#0a0;
	}
	
	.ngword{color:{$STYLE['read_ngword']};}
	
	.aborned{ font-size:1px; }
	.aborned span{ display:none; }
	
	.respopup{ 	/* ���p���X�|�b�v�A�b�v */
		position:absolute;
		visibility:hidden; /* ���i�͉B���Ă���*/
		background-color:#ffffcc;
		border:1px black solid;
		font-size:{$STYLE['respop_fontsize']};
		padding:8px;
		line-height:120%;
	}
	
	#iframespace{ /* HTML�|�b�v�A�b�v�X�y�[�X */
		position:absolute;
		z-index:100;
		/*border:solid 1px;*/
	}

	#closebox{
		width:14px;
		height:14px;
		position:absolute;
		z-index:101;
		border:solid 2px;
		padding:1px;
		line-height:100%;
		background-color:#ceddf7;
	}
	
	div#kakiko{
		display:none;
	}

	/* ���X�̂��΂₳ */
	span.spd{
		font-size:8pt;
		color:#777;
	}	
-->
</style>

EOP;
?>