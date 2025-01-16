package com.example.homeworkthree;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Fruit {
    public String name;
    public String description;

    public Fruit(String name, String description) {
        this.name = name;
        this.description = description;
    }

    private final static String[] nameArray = {
            "苹果", "香蕉", "柠檬", "山竹", "西瓜", "草莓", "葡萄", "芒果", "菠萝", "猕猴桃",
            "橙子", "梨", "桃子", "李子", "樱桃", "榴莲", "椰子", "甘蔗", "柿子", "石榴"
    };

    private final static String[] descArray = {
            "苹果（Malus pumila Mill.），蔷薇科苹果属落叶乔木植物，茎干较高，小枝短而粗，呈圆柱形；叶片椭圆形，表面光滑，边缘有锯齿，叶柄粗壮；花朵较小呈伞状，淡粉色，表面有绒毛；果实较大，呈扁球形，果梗短粗；花期5月；果期7~10月。苹果名称最早是见于明代王世懋的《学圃余疏》“北土之苹婆果，即花红一种之变也。",
            "香蕉（Musa nana Lour.），芭蕉科芭蕉属多年生草本植物，植株丛生，有匐匍茎；假茎浓绿有黑色斑点；叶片长圆形，上面为深绿色，无白粉，下面浅绿色；花朵为乳白色或淡紫色；果实呈弯曲的弓状，有棱，果皮为青绿色，成熟后变黄；果肉松软，黄白色，味甜香味浓，无种子",
            "柠檬（Citrus × limon (Linnaeus) Osbeck），芸香科柑橘属木本植物，枝少刺或近于无刺，嫩叶及花芽暗紫红色；叶为卵形或椭圆形；花瓣外面为淡紫红色，内面白色；果实为椭圆形或卵形，果皮厚，柠檬黄色；花期4~5月；果期9~11月。",
            "山竹（Garcinia mangostana L.）是藤黄科藤黄属植物，原产于马鲁古，亚洲和非洲热带地区广泛栽培，喜欢有机物丰富、pH值在5~6.5的砂壤。果皮厚，紫红色，果肉白色，味甜。",
            "西瓜（Citrullus lanatus (Thunb.) Matsum. & Nakai），是葫芦科西瓜属一年生蔓生藤本植物，形态一般近似于球形或椭圆形，颜色有深绿、浅绿或带有黑绿条带或斑纹；瓜籽多为黑色，呈椭圆形，头尖；茎枝粗壮，有淡黄褐色的柔毛；叶片如纸，呈三角状卵形，边缘呈波状。花果期5—6月。 因9世纪自西域传入中国，故名西瓜。",
            "草莓（Fragaria × ananassa Duch.），蔷薇科草莓属多年生草本植物，植株矮小，匍匐茎发达；叶片为三出复叶，小叶呈卵圆形，边缘有锯齿；花为白色，聚伞花序；果实为聚合果，红色，表面有许多小点（瘦果）；味甜酸。",
            "葡萄（Vitis vinifera L.），葡萄科葡萄属木质藤本植物，小枝圆柱形，有纵棱纹；叶卵圆形，常3-5裂；花序为圆锥花序，与叶对生；果实为球形或椭圆形，成熟时呈紫黑色、红色或白色等；味甜或酸甜。",
            "芒果（Mangifera indica L.），漆树科芒果属常绿乔木，树冠宽大；叶革质，长圆形或披针形；花小，黄色或淡红色，圆锥花序；果实为核果，成熟时呈黄色或橙黄色，果肉多汁，味甜。",
            "菠萝（Ananas comosus (L.) Merr.），菠萝科菠萝属多年生草本植物，茎短粗；叶剑状，边缘有锯齿；花为穗状花序，聚花果肉质，黄色，味甜酸。",
            "猕猴桃（Actinidia chinensis Planch.），猕猴桃科猕猴桃属落叶藤本植物，果皮通常覆盖浓密绒毛，果肉绿色或黄色，富含维生素C。",
            "橙子（Citrus sinensis (L.) Osbeck），芸香科柑橘属常绿乔木，果皮橙黄色，果肉多汁，味甜或酸甜。",
            "梨（Pyrus pyrifolia (Burm. f.) Nakai），蔷薇科梨属落叶乔木，果实通常呈圆形或梨形，果皮绿色、黄色或褐色，果肉白色，多汁。",
            "桃子（Prunus persica (L.) Batsch），蔷薇科李属落叶乔木，果实通常呈球形或扁球形，果皮有毛，果肉白色或黄色，味甜。",
            "李子（Prunus salicina Lindl.），蔷薇科李属落叶乔木，果实通常呈球形或椭圆形，果皮红色、紫色或黄色，果肉黄色或红色，味酸甜。",
            "樱桃（Cerasus pseudocerasus (Lindl.) G. Don），蔷薇科樱属落叶乔木，果实小，红色，味甜或酸甜。",
            "榴莲（Durio zibethinus Murr.），锦葵科榴莲属常绿乔木，果实大，外壳多刺，果肉黄色，气味浓烈。",
            "椰子（Cocos nucifera L.），棕榈科椰子属常绿乔木，果实外壳坚硬，内有白色果肉和椰子水。",
            "甘蔗（Saccharum officinarum L.），禾本科甘蔗属多年生草本植物，茎粗壮，富含糖分。",
            "柿子（Diospyros kaki L.f.），柿树科柿属落叶乔木，果实成熟时呈橙黄色或红色，味甜。",
            "石榴（Punica granatum L.），石榴科石榴属落叶灌木或小乔木，果实外皮坚硬，内有许多红色种子，味酸甜。"
    };

    public static List<Fruit> getDefaultList() {
        List<Fruit> list = new ArrayList<>();
        // 确保数组长度一致，避免越界异常
        int minLength = Math.min(nameArray.length, descArray.length);
        for (int i = 0; i < minLength; i++) {
            list.add(new Fruit(nameArray[i], descArray[i]));
        }
        return list;
    }

    public static Map<String, String> getDescMap() {
        // 使用正确的长度创建 HashMap
        int minLength = Math.min(nameArray.length, descArray.length);
        Map<String, String> map = new HashMap<>(minLength);
        for (int i = 0; i < minLength; i++) {
            map.put(nameArray[i], descArray[i]);
        }
        return map;
    }
}