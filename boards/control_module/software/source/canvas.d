
import coordinate;

enum StorageType {
	RowFirst,
	ColumnFirst,
	Other
}

void blit(FromCanvas, ToCanvas)(
		FromCanvas from, Coordinate!() from_start, Coordinate!() size,
		ToCanvas to_, Coordinate!() to_start
)
if(FromCanvas.storage_type == StorageType.ColumnFirst
		&& ToCanvas.storage_type == StorageType.ColumnFirst)
{
	import core.stdc.string: memcpy;

	int n_from = from.size.column;
	int n_to = to_.size.column;

	ubyte *buf_from = from.buffer.ptr + from_start.row * n_from + from_start.column;
	ubyte *buf_to   = to_.buffer.ptr + to_start.row * n_to + to_start.column;

	//for(int row = from.row(); row < (from + size).row(); ++row) {
	for(int i = 0; i < size.row; i++) {
		memcpy(
				buf_to + i * n_to,
				buf_from + i * n_from,
				size.column
		);
	}
}


