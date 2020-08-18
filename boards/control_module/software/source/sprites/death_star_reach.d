import string_canvas: strings_to_frames;
import display: Color;
import std.datetime: Duration, msecs;
import std.typecons: Flag, Yes, No;
import coordinate: Coord;
import sprite: Sprite;

static death_star_reach_frames = strings_to_frames(
		Coord(16, 40),
		[
			' ': Color.BLACK,
			'R': Color.RED,
			'r': Color.DARK_RED,
			'G': Color.GREEN,
			'g': Color.DARK_GREEN,
			'Y': Color.YELLOW,
			'O': Color.ORANGE,
			'o': Color.BLOOD_ORANGE,
		],
		[
q"X
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR         
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR        
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR       
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR       
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR      
rrrrrGGrGGGrrrrrrrrrrrrrrrrrrrrrrR      
rrrrGrrgggggrrrrrrrrrrrrrrrrrrrrrR      
rrrGrrGGGGGGGrrrrrrrrrrrrrrrrrrrrrR     
rrrGGrgggggggrrrrrrrrrrrrrrrrrrrrrR     
rrrGrrGGGGGGGrrrrrrrrrrrrrrrrrrrrrR     
rrrrGrrgggggrrrrrrrrrrrrrrrrrrrrrR      
rrrrrGGrGGGrrrrrrrrrrrrrrrrrrrrrrR      
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR      
rrrrrrrrrrrrrrrrrrrrrrrrrr Y YY Y   Y   
rrrrrrrrrrrrrrrrrrrrrrrrrr Y YY YY  Y   
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR        
X", q"X
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR         
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR        
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR       
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR       
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR      
rrrrrGGrGGGGrrrrrrrrrrrrrrrrrrrrrR      
rrrrGrrGgggggrrrrrrrrrrrrrrrrrrrrR      
rrrGrrGGGGGGGGrrrrrrrrrrrrrrrrrrrrR     
rrrGGrGgggggggrrrrrrrrrrrrrrrrrrrrR     
rrrGrrGGGGGGGGrrrrrrrrrrrrrrrrrrrrR     
rrrrGrrGgggggrrrrrrrrrrrrrrrrrrrrR      
rrrrrGGrGGGGrrrrrrrrrrrrrrrrrrrrrR      
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR      
rrrrrrrrrrrrrrrrrrrrrrrrrr Y YY Y   Y   
rrrrrrrrrrrrrrrrrrrrrrrrrr Y YY YY YY   
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR        
X", q"X
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR         
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR        
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR       
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR       
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR      
rrrrrGGrGrGGGrrrrrrrrrrrrrrrrrrrrR      
rrrrGrrGrgggggrrrrrrrrrrrrrrrrrrrR      
rrrGrrGrGGGGGGGrrrrrrrrrrrrrrrrrrrR     
rrrGGrGrgggggggrrrrrrrrrrrrrrrrrrrR     
rrrGrrGrGGGGGGGrrrrrrrrrrrrrrrrrrrR     
rrrrGrrGrgggggrrrrrrrrrrrrrrrrrrrR      
rrrrrGGrGrGGGrrrrrrrrrrrrrrrrrrrrR      
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR      
rrrrrrrrrrrrrrrrrrrrrrrrrr Y YY Y  Y    
rrrrrrrrrrrrrrrrrrrrrrrrrr Y YY YY YY   
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR        
X", q"X
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR         
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR        
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR       
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR       
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR      
rrrrrGGrGGrGGGrrrrrrrrrrrrrrrrrrrR      
rrrrGrrGrrgggggrrrrrrrrrrrrrrrrrrR      
rrrGrrGrrGGGGGGGrrrrrrrrrrrrrrrrrrR     
rrrGGrGrrgggggggrrrrrrrrrrrrrrrrrrR     
rrrGrrGrrGGGGGGGrrrrrrrrrrrrrrrrrrR     
rrrrGrrGrrgggggrrrrrrrrrrrrrrrrrrR      
rrrrrGGrGGrGGGrrrrrrrrrrrrrrrrrrrR      
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR      
rrrrrrrrrrrrrrrrrrrrrrrrrr Y  Y YY YY   
rrrrrrrrrrrrrrrrrrrrrrrrrr Y  Y YY YY   
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR        
X", q"X
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR         
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR        
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR       
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR       
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR      
rrrrrGGrGGrGGGGrrrrrrrrrrrrrrrrrrR      
rrrrGrrGrrGgggggrrrrrrrrrrrrrrrrrR      
rrrGrrGrrGGGGGGGGrrrrrrrrrrrrrrrrrR     
rrrGGrGrrGgggggggrrrrrrrrrrrrrrrrrR     
rrrGrrGrrGGGGGGGGrrrrrrrrrrrrrrrrrR     
rrrrGrrGrrGgggggrrrrrrrrrrrrrrrrrR      
rrrrrGGrGGrGGGGrrrrrrrrrrrrrrrrrrR      
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR      
rrrrrrrrrrrrrrrrrrrrrrrrrr Y  Y YY  Y   
rrrrrrrrrrrrrrrrrrrrrrrrrr Y  Y YY  Y   
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR        
X", q"X
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR         
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR        
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR       
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR       
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR      
rrrrrGGrGGrGrGGGrrrrrrrrrrrrrrrrrR      
rrrrGrrGrrGrgggggrrrrrrrrrrrrrrrrR      
rrrGrrGrrGrGGGGGGGrrrrrrrrrrrrrrrrR     
rrrGGrGrrGrgggggggrrrrrrrrrrrrrrrrR     
rrrGrrGrrGrGGGGGGGrrrrrrrrrrrrrrrrR     
rrrrGrrGrrGrgggggrrrrrrrrrrrrrrrrR      
rrrrrGGrGGrGrGGGrrrrrrrrrrrrrrrrrR      
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR      
rrrrrrrrrrrrrrrrrrrrrrrrrr Y  Y YY  Y   
rrrrrrrrrrrrrrrrrrrrrrrrrr Y  Y YY YY   
rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrR        
X",
	]
);

Sprite death_star_reach_sprite;
static this() {
	death_star_reach_sprite = new Sprite(death_star_reach_frames, 1000.msecs, Yes.loop);
}
