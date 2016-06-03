<?php
/**
 * rep2expack - pager for Mobile
 */

// {{{ get_read_jump()

/**
 * �y�[�W�J�ڗp��HTML�v�f���擾����
 */
function get_read_jump(ThreadRead $aThread, $label, $use_onchange)
{
    global $_conf;

    $resFilter = ResFilter::getFilter();

    if ($resFilter && $resFilter->hasWord()) {
        $jump = _get_read_jump_filter($aThread, $resFilter, $use_onchange);
    } else {
        $jump = _get_read_jump($aThread, $use_onchange);
    }

    if ($use_onchange) {
        return $jump;
    } else {
        return "<form method=\"get\" action=\"{$_conf['read_php']}\" accept-charset=\"{$_conf['accept_charset']}\">{$label}{$jump}</form>";
    }
}

// }}}
// {{{ _get_read_jump()

/**
 * �y�[�W�J�ڗp��HTML�v�f���擾���� (�ʏ펞)
 */
function _get_read_jump(ThreadRead $aThread, $use_onchange)
{
    global $_conf;

    $rpp = (int)$_conf['mobile.rnum_range'];

    if ($rpp < 1) {
        $options = '<option value="1">$_conf[&#39;mobile.rnum_range&#39;] �̒l���s���ł�</option>';
    } else {
        //if ($aThread->resrange['start'] != 1 && $aThread->resrange['start'] % $rpp) {
        if (($aThread->resrange['start'] - 1) % $rpp) {
            $ls = p2h($aThread->ls);
            $options = "<option value=\"{$ls}\" selected>{$ls}</option>";
        } else {
            $options = '';
        }

        /*$optgroup = $rpp * 5;
        if ($optgroup >= $aThread->rescount) {
            $optgroup = 0; 
        }*/

        $rescount = $aThread->rescount;
        $pages = ceil($rescount / $rpp);

        for ($i = 0; $i < $pages; $i++) {
            $j = $i + 1;
            $k = $i * $rpp + 1;
            $l = $j * $rpp + 1;
            if ($l > $rescount) {
                $l = $rescount;
            }

            /*if ($k > 1) {
                $k--;
            }*/

            /*if ($optgroup && $i % $optgroup == 0) {
                if ($i) {
                    $options .= '</optgroup>';
                }
                $options .= "<optgroup label=\"{$j}-\">";
            }*/

            if ($k == $l) {
                $m = (string)$k;
                $n = "{$m}n";
            } else {
                $m = "{$k}-";
                $n = "{$m}{$l}n";
            }

            if ($k == $aThread->resrange['start']) {
                $options .= "<option value=\"{$n}\" selected>{$m}</option>";
            } else {
                $options .= "<option value=\"{$n}\">{$m}</option>";
            }
        }

        /*if ($optgroup) {
            $options .= '</optgroup>';
        }*/
    }

    if ($use_onchange) {
        return _get_read_jump_js($aThread, $options);
    } else {
        return _get_read_jump_form($aThread, $options);
    }
}

// }}}
// {{{ _get_read_jump_filter()

/**
 * �y�[�W�J�ڗp��HTML�v�f���擾���� (������)
 */
function _get_read_jump_filter(ThreadRead $aThread, ResFilter $resFilter, $use_onchange)
{
    global $_conf;

    if ($_conf['mobile.rnum_range'] < 1) {
        $options = '<option value="1">$_conf[&#39;mobile.rnum_range&#39;] �̒l���s���ł�</option>';
    } else {
        $options = '';
        $filter_hits = $resFilter->hits;

        /*$optgroup = $_conf['mobile.rnum_range'] * 5;
        if ($optgroup >= $filter_hits) {
            $optgroup = 0; 
        }*/

        $pages = ceil($filter_hits / $_conf['mobile.rnum_range']);

        for ($i = 0; $i < $pages; $i++) {
            $j = $i + 1;
            $k = $i * $_conf['mobile.rnum_range'] + 1;
            $l = $j * $_conf['mobile.rnum_range'];
            if ($l > $filter_hits) {
                $l = $filter_hits;
            }

            /*if ($optgroup && $i % $optgroup == 0) {
                if ($i) {
                    $options .= '</optgroup>';
                }
                $options .= "<optgroup label=\"{$j}-\">";
            }*/

            $m = ($k == $l) ? "$k" : "{$k}-"; //"{$k}-{$l}";

            if ($j == $resFilter->range['page']) {
                $options .= "<option value=\"{$j}\" selected>{$m}</option>";
            } else {
                $options .= "<option value=\"{$j}\">{$m}</option>";
            }
        }

        /*if ($optgroup) {
            $options .= '</optgroup>';
        }*/
    }

    if ($use_onchange) {
        return _get_read_jump_filter_js($aThread, $options);
    } else {
        return _get_read_jump_filter_form($aThread, $options);
    }
}

// }}}
// {{{ _get_read_jump_form()

/**
 * �y�[�W�J�ڗp�t�H�[���v�f���擾���� (�ʏ펞)
 */
function _get_read_jump_form(ThreadRead $aThread, $options)
{
    global $_conf;

    $word = p2h($GLOBALS['word']);

    return <<<EOP
<input type="hidden" name="host" value="{$aThread->host}">
<input type="hidden" name="bbs" value="{$aThread->bbs}">
<input type="hidden" name="key" value="{$aThread->key}">
<select name="ls">{$options}</select><input type="submit" value="GO">
<input type="hidden" name="offline" value="1">{$_conf['k_input_ht']}
EOP;
}

// }}}
// {{{ _get_read_jump_filter_form()

/**
 * �y�[�W�J�ڗp�t�H�[���v�f���擾���� (������)
 */
function _get_read_jump_filter_form(ThreadRead $aThread, $options)
{
    global $_conf, $hd;

    return <<<EOP
<input type="hidden" name="host" value="{$aThread->host}">
<input type="hidden" name="bbs" value="{$aThread->bbs}">
<input type="hidden" name="key" value="{$aThread->key}">
<input type="hidden" name="word" value="{$hd['word']}">
<input type="hidden" name="method" value="{$hd['method']}">
<input type="hidden" name="field" value="{$hd['field']}">
<input type="hidden" name="match" value="{$hd['match']}">
<select name="page">{$options}</select><input type="submit" value="GO">
<input type="hidden" name="offline" value="1">
{$_conf['detect_hint_input_ht']}{$_conf['k_input_ht']}
EOP;
}

// }}}
// {{{ _get_read_jump_js()

/**
 * �I�v�V�������I�����ꂽ�Ƃ��ɑJ�ڂ���select�v�f���擾���� (�ʏ펞)
 */
function _get_read_jump_js(ThreadRead $aThread, $options)
{
    global $_conf;

    return <<<EOP
<select onchange="location.href = '{$_conf['read_php']}?host={$aThread->host}&amp;bbs={$aThread->bbs}&amp;key={$aThread->key}&amp;ls=' + this.options[this.selectedIndex].value + '&amp;offline=1{$_conf['k_at_a']}';">{$options}</select>
EOP;
}

// }}}
// {{{ _get_read_jump_filter_js()

/**
 * �I�v�V�������I�����ꂽ�Ƃ��ɑJ�ڂ���select�v�f (������)
 */
function _get_read_jump_filter_js(ThreadRead $aThread, $options)
{
    global $_conf;

    return <<<EOP
<select onchange="location.href = '{$_conf['read_php']}{$_conf['filter_q']}' + this.options[this.selectedIndex].value + '{$_conf['k_at_a']}';">{$options}</select>
EOP;
}

// }}}

/*
 * Local Variables:
 * mode: php
 * coding: cp932
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
// vim: set syn=php fenc=cp932 ai et ts=4 sw=4 sts=4 fdm=marker: