
$(document).ready(function(){

$("#file-check").click(function() {
      $(".pininput").css("opacity", .5);
});

$("#fselfcap-check, #mutual-check").click(function() {
      $(".pininput").css("opacity", 1);
});

/*  $('.increase-controls').on('click',function(){
      var $qty=$(this).closest("div").find('#adjust-number');
      var currentVal = parseInt($qty.val());
      if (!isNaN(currentVal)) {
          $qty.val(currentVal + 1);
      }
      var $ctrl = $('<input/>').attr({ type: 'checkbox', name:'chk'}).addClass("chk");
      $(".electrodes-field").append($ctrl);
      $(".electrodes-field").append("<p>El_1</p>");
  });

  $('.increase-electrodes').on('click',function(){
      var $qty=$(this).closest("div").find('#adjust-number');
      var currentVal = parseInt($qty.val());
      if (!isNaN(currentVal)) {
          $qty.val(currentVal + 1);
      }
  });*/

$(".erase-tag").click(function(){
    $(this).parent('div').fadeOut(400);
});

$(".tag-arotary").click(function(){
    $(".tag-arotary").removeClass('active-tag');
    $(this).addClass('active-tag');
});

$(".tag-electrode").click(function(){
    $(".tag-electrode").removeClass('active-tag');
    $(this).addClass('active-tag');
});


$("nav li").click(function() {
  $("nav li").removeClass("active-link");
  $(this).addClass("active-link");
});

$("#modules-link").click(function() {
  $(" .touchsw, .tables-ext, .controls, .electrodes, .debug, .genconf").fadeOut(100);
  $(".modules").fadeIn(100);
});

$("#theme-link").click(function() {
  $(".modules, .touchsw, .controls, .electrodes, .debug, .genconf").fadeOut(100);
  $(".tables-ext").fadeIn(100);
});

$("#touchsw-link").click(function() {
  $(".modules, .tables-ext, .controls, .electrodes, .debug, .genconf").fadeOut(100);
  $(".touchsw").fadeIn(100);
});

$("#controls-link").click(function() {
  $(".modules, .tables-ext, .touchsw, .electrodes, .debug, .genconf").fadeOut(100);
  $(".controls").fadeIn(100);
});

$("#electrodes-link").click(function() {
  $(".modules, .tables-ext, .controls, .touchsw, .debug, .genconf").fadeOut(100);
  $(".electrodes").fadeIn(100);
});

$("#debug-link").click(function() {
  $(".modules, .tables-ext, .controls, .electrodes, .touchsw, .genconf").fadeOut(100);
  $(".debug").fadeIn(100);
});

$("#generated-config").click(function() {
  $(".modules, .tables-ext, .controls, .electrodes, .touchsw, .debug").fadeOut(100);
  $(".genconf").fadeIn(100);
});


$("#dvolt").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").fadeIn(150).addClass("rotated");
    $(".popup").not("#popup-dvolt").slideUp(300);
    $(".theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-dvolt").slideDown(300);
    $("#dvolt").addClass("dvolt-active");
    $("#dvolt").addClass("selected");
    $(".theme-item-wrap div").not(this).addClass("opaq025");
});

$("#clock").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").addClass("rotated");
    $(".popup").not("#popup-clock").slideUp(300);
    $(".theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-clock").slideDown(300);
    $("#clock").addClass("clock-active");
    $("#clock").addClass("selected");
    $(".theme-item-wrap div").not(this).addClass("opaq025");
});

$("#comparator").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").addClass("rotated");
    $(".popup").not("#popup-comparator").slideUp(300);
    $(".theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-comparator").slideDown(300);
    $("#comparator").addClass("comparator-active");
    $("#comparator").addClass("selected");
    $(".theme-item-wrap div").not(this).addClass("opaq025");
});

$("#m-star").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").addClass("rotated");
    $(".popup").not("#popup-m-star").slideUp(300);
    $(".theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-m-star").slideDown(300);
    $("#m-star").addClass("m-star-active");
    $("#m-star").addClass("selected");
    $(".theme-item-wrap div").not(this).addClass("opaq025");
});

$("#s-star").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").addClass("rotated");
    $(".popup").not("#popup-s-star").slideUp(300);
    $(".theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-s-star").slideDown(300);
    $("#s-star").addClass("s-star-active");
    $("#s-star").addClass("selected");
    $(".theme-item-wrap div").not(this).addClass("opaq025");
});

$("#ssc").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").addClass("rotated");
    $(".popup").not("#popup-ssc").slideUp(300);
    $(".theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-ssc").slideDown(300);
    $("#ssc").addClass("ssc-active");
    $("#ssc").addClass("selected");
    $(".theme-item-wrap div").not(this).addClass("opaq025");
});

$("#sinc").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").addClass("rotated");
    $(".popup").not("#popup-sinc").slideUp(300);
    $(".theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-sinc").slideDown(300);
    $("#sinc").addClass("sinc-active");
    $("#sinc").addClass("selected");
    $(".theme-item-wrap div").not(this).addClass("opaq025");
});

$("#control").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").addClass("rotated");
    $(".popup").not("#popup-control").slideUp(300);
    $(".theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-control").slideDown(300);
    $("#control").addClass("control-active");
    $("#control").addClass("selected");
    $(".theme-item-wrap div").not(this).addClass("opaq025");
});

$("#clk").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").addClass("rotated");
    $(".popup").not("#popup-clk").slideUp(300);
    $(".cs-theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-clk").slideDown(300);
    $("#clk").addClass("clk-active");
    $("#clk").addClass("selected");
    $(".cs-theme-item-wrap div").not(this).addClass("opaq025");
});

$("#controller").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").addClass("rotated");
    $(".popup").not("#popup-controller").slideUp(300);
    $(".cs-theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-controller").slideDown(300);
    $("#controller").addClass("controller-active");
    $("#controller").addClass("selected");
    $(".cs-theme-item-wrap div").not(this).addClass("opaq025");
});

$("#counter").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").addClass("rotated");
    $(".popup").not("#popup-counter").slideUp(300);
    $(".cs-theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-counter").slideDown(300);
    $("#counter").addClass("counter-active");
    $("#counter").addClass("selected");
    $(".cs-theme-item-wrap div").not(this).addClass("opaq025");
});

$("#fifo").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").addClass("rotated");
    $(".popup").not("#popup-fifo").slideUp(300);
    $(".cs-theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-fifo").slideDown(300);
    $("#fifo").addClass("fifo-active");
    $("#fifo").addClass("selected");
    $(".cs-theme-item-wrap div").not(this).addClass("opaq025");
});

$("#int").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").addClass("rotated");
    $(".popup").not("#popup-int").slideUp(300);
    $(".cs-theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-int").slideDown(300);
    $("#int").addClass("int-active");
    $("#int").addClass("selected");
    $(".cs-theme-item-wrap div").not(this).addClass("opaq025");
});

$("#register").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").addClass("rotated");
    $(".popup").not("#popup-register").slideUp(300);
    $(".cs-theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-register").slideDown(300);
    $("#register").addClass("register-active");
    $("#register").addClass("selected");
    $(".cs-theme-item-wrap div").not(this).addClass("opaq025");
});

$("#oscilator").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").addClass("rotated");
    $(".popup").not("#popup-oscilator").slideUp(300);
    $(".cs-theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-oscilator").slideDown(300);
    $("#oscilator").addClass("oscilator-active");
    $("#oscilator").addClass("selected");
    $(".cs-theme-item-wrap div").not(this).addClass("opaq025");
});

$("#switch").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").addClass("rotated");
    $(".popup").not("#popup-switch").slideUp(300);
    $(".cs-theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-switch").slideDown(300);
    $("#switch").addClass("switch-active");
    $("#switch").addClass("selected");
    $(".cs-theme-item-wrap div").not(this).addClass("opaq025");
});

$("#switch-small").click(function() {
    $("#show-popup").fadeOut(0);
    $("#popup-load").addClass("rotated");
    $(".popup").not("#popup-switch-small").slideUp(300);
    $(".cs-theme-item-wrap div").not(this).removeClass("selected");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $("#popup-switch-small").slideDown(300);
    $("#switch-small").addClass("switch-small-active");
    $("#switch-small").addClass("selected");
    $(".cs-theme-item-wrap div").not(this).addClass("opaq025");
});

$("#show-popup").click(function() {
    $(this).fadeOut(0);
    $(".theme-item-wrap div").not(".selected").toggleClass("opaq025");
    $(".selected").removeClass("selected");
});

$(".change-tsi").click(function() {
    $(".change-tsi-cs p").removeClass("active-tsi-cs");
    $(this).addClass("active-tsi-cs");
    $(".cs-case").fadeOut(100);
    $(".tsi-case").fadeIn(100);
    $(".popup").slideUp(300);
    $(".theme-item-wrap div").removeClass("selected");
    $(".cs-theme-item-wrap div").removeClass("selected");
    $(".theme-item-wrap div").removeClass("opaq025");
    $(".cs-theme-item-wrap div").removeClass("opaq025");
    $(".theme-item").removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
});

$(".change-cs").click(function() {
    $(".change-tsi-cs p").removeClass("active-tsi-cs");
    $(this).addClass("active-tsi-cs");
    $(".tsi-case").fadeOut(100);
    $(".cs-case").fadeIn(100);
    $(".popup").slideUp(300);
    $(".theme-item-wrap div").removeClass("selected");
    $(".cs-theme-item-wrap div").removeClass("selected");
    $(".theme-item-wrap div").removeClass("opaq025");
    $(".cs-theme-item-wrap div").removeClass("opaq025");
    $(".cs-theme-item").removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $(".theme-item").removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
});


  $(".theme-all, .cs-theme-all, .rotate180").click(function() {
    $("#show-popup").fadeIn(0);
    $(".popup").slideUp(300);
    $("#popup-load").removeClass("rotated");
    $(".theme-item").not(this).removeClass("ssc-active sinc-active control-active m-star-active s-star-active clock-active comparator-active dvolt-active");
    $(".cs-theme-item").not(this).removeClass("clk-active controller-active counter-active fifo-active int-active register-active oscilator-active switch-active switch-small-active");
    $(".theme-item-wrap div").removeClass("selected");
    $(".cs-theme-item-wrap div").removeClass("selected");
    $(".theme-item-wrap div").removeClass("opaq025");
    $(".cs-theme-item-wrap div").removeClass("opaq025");
  });

});
