#!/usr/bin/perl -w

use Tk;
use Tk::X11Font;

# -------------------------------------------------------------------------

sub draw {
	my ( $a, $b, $c ) = @_; 

	my ($x, $y) = ( $margin, $margin );

	$canvas->createRectangle( $x, $y, $x+$size, $y+$size, -fill=>"white" );
	$canvas->createText( $x+$mid, $y+$mid, -text=>"$a", -font=>$font);
	$x += $size + $margin;

	$canvas->createRectangle( $x, $y, $x+$size, $y+$size, -fill=>"red" );
	$canvas->createText( $x+$mid, $y+$mid, -text=>"$b", -font=>$font);
	$x += $size + $margin;

	my @color = ("yellow", "green", "blue", "black", "black", "black");

	$canvas->createRectangle( $x, $y, $x+$size, $y+$size, 
		-fill=>$color[$c-1] );
}

sub roll {
	my ($a, $b, $c) = ( int(rand(6)+1), int(rand(6)+1), int(rand(6)+1) );
	draw ($a, $b, $c);
}

# -------------------------------------------------------------------------

my $size = 450;
my $margin = 10;
my $mid = $size/2;

my $wwidth = 3*$size + 4*$margin;
my $wheight = $size + 2*$margin;

# Create MainWindow and configure:
my $mw = MainWindow->new;
$mw->configure( -width=>$wwidth, -height=>$wheight );
$mw->resizable( 0, 0 ); # not resizable in any direction

# Create and configure the canvas:
my $canvas = $mw->Canvas( -cursor=>"crosshair", -background=>"white",
	-width=>$wwidth, -height=>$wheight )->pack;

# get a font that's big
my $font = $canvas->X11Font(foundry => 'adobe', 
	family  => 'times', point => 2000);

# add a button
$mw->Button( -text => "Roll", -command => sub { roll(); },
	-height => 40, -width => 100)->pack;

# run user events
MainLoop;
