//  Copyright (c) 2007-2009 Fredrik Mellbin
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#ifndef INDEXING_H
#define	INDEXING_H

#include "utils.h"
#include "wave64writer.h"

#ifdef _WIN32
#	define _WIN32_DCOM
#	include <windows.h>
#	include <tchar.h>
#	include <atlbase.h>
#	include <dshow.h>
#	include <initguid.h>
#	include "CoParser.h"
#	include "guids.h"
#endif

#define INDEXVERSION 26
#define INDEXID 0x53920873

struct IndexHeader {
	uint32_t Id;
	uint32_t Version;
	uint32_t Tracks;
	uint32_t Decoder;
	uint32_t LAVUVersion;
	uint32_t LAVFVersion;
	uint32_t LAVCVersion;
	uint32_t LSWSVersion;
	uint32_t LPPVersion;
};

class SharedAudioContext {
public:
	Wave64Writer *W64W;
	AVCodecContext *CTX;
	int64_t CurrentSample;

	SharedAudioContext();
	~SharedAudioContext();
};

class FFAudioContext : public SharedAudioContext {
};

class MatroskaAudioContext : public SharedAudioContext {
public:
	CompressedStream *CS;
	uint8_t *CodecPrivate;

	MatroskaAudioContext();
	~MatroskaAudioContext();
};

class FFIndexer {
protected:
	int IndexMask;
	int DumpMask;
	bool IgnoreDecodeErrors;
	TIndexCallback IC;
	void *ICPrivate;
	TAudioNameCallback ANC;
	void *ANCPrivate;
	const char *SourceFile;
	int16_t *DecodingBuffer; 

	bool WriteAudio(SharedAudioContext &AudioContext, FFIndex *Index, int Track, int DBSize, char *ErrorMsg, unsigned MsgSize);
public:
	static FFIndexer *CreateFFIndexer(const char *Filename, char *ErrorMsg, unsigned MsgSize);
	FFIndexer();
	virtual ~FFIndexer();
	void SetIndexMask(int IndexMask);
	void SetDumpMask(int DumpMask);
	void SetIgnoreDecodeErrors(bool IgnoreDecodeErrors);
	void SetProgressCallback(TIndexCallback IC, void *ICPrivate);
	void SetAudioNameCallback(TAudioNameCallback ANC, void *ANCPrivate);
	virtual FFIndex *DoIndexing(char *ErrorMsg, unsigned MsgSize) = 0;
	virtual int GetNumberOfTracks() = 0;
	virtual FFMS_TrackType GetTrackType(int Track) = 0;
	virtual const char *GetTrackCodec(int Track) = 0;
};

class FFLAVFIndexer : public FFIndexer {
private:
	bool IsIndexing;
	AVFormatContext *FormatContext;
public:
	FFLAVFIndexer(const char *Filename, AVFormatContext *FormatContext, char *ErrorMsg, unsigned MsgSize);
	~FFLAVFIndexer();
	FFIndex *DoIndexing(char *ErrorMsg, unsigned MsgSize);
	int GetNumberOfTracks();
	FFMS_TrackType GetTrackType(int Track);
	const char *GetTrackCodec(int Track);
};

class FFMatroskaIndexer : public FFIndexer {
private:
	MatroskaFile *MF;
	MatroskaReaderContext MC;
	AVCodec *Codec[32];
public:
	FFMatroskaIndexer(const char *Filename, char *ErrorMsg, unsigned MsgSize);
	~FFMatroskaIndexer();
	FFIndex *DoIndexing(char *ErrorMsg, unsigned MsgSize);
	int GetNumberOfTracks();
	FFMS_TrackType GetTrackType(int Track);
	const char *GetTrackCodec(int Track);
};

#ifdef HAALISOURCE

class FFHaaliIndexer : public FFIndexer {
private:
	int SourceMode;
	CComPtr<IMMContainer> pMMC;
	int NumTracks;
	FFMS_TrackType TrackType[32];
	AVCodec *Codec[32];
	uint8_t *CodecPrivate[32];
	int CodecPrivateSize[32];
	int64_t Duration;
public:
	FFHaaliIndexer(const char *Filename, int SourceMode, char *ErrorMsg, unsigned MsgSize);
	~FFHaaliIndexer();
	FFIndex *DoIndexing(char *ErrorMsg, unsigned MsgSize);
	int GetNumberOfTracks();
	FFMS_TrackType GetTrackType(int Track);
	const char *GetTrackCodec(int Track);
};

#endif // HAALISOURCE

#endif
