#include<pocketsphinx.h>
//#define MODELDIR "/usr/local/share/pocketsphinx/model"
int
main(int argc, char* argv[])
{
  if(argc != 2){
    fprintf(stderr, "Inappropriate number of args. Please read README.txt");
    return -1;
  }
  ps_decoder_t* ps = NULL;
  cmd_ln_t* config = NULL;
  FILE *fh;
  char const *hyp, *uttid;
  int16 buf[512];
  int rv;
  int32 score;

  config = cmd_ln_init(NULL, ps_args(), TRUE,
            "-hmm", MODELDIR "/en-us/en-us",
            "-lm", MODELDIR "/en-us/en-us.lm.bin",
            "-dict", MODELDIR "/en-us/cmudict-en-us.dict",
            NULL);


  if(config == NULL) {
    fprintf(stderr, "Failed to create config object, see log for details\n");
    return -1;
  }

  ps = ps_init(config); //decoder

  if (ps == NULL) {
    fprintf(stderr, "Failed to create recognizer, see log for details\n");
    return -1;
  }

  fh = fopen(argv[1], "rb");
    if (fh == NULL) {
        fprintf(stderr, "Unable to open input file %s\n",argv[1]);
        return -1;
    }

  rv = ps_start_utt(ps);

  while(!feof(fh)) {
    size_t nsamp;
    nsamp = fread(buf, 2, 512, fh);
    rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
  }

  rv = ps_end_utt(ps);

  hyp = ps_get_hyp(ps, &score);
 
  printf("Recognized: %s\n", hyp);

  FILE *fp;
  fp = fopen("transcription.txt", "w");
  fprintf(fp, "Recognized: %s\n", hyp);

  fclose(fp);
  fclose(fh);
  ps_free(ps);
  cmd_ln_free_r(config);



  return 0;
}
