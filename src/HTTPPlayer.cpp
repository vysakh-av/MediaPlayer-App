#include "HTTPPlayer.h"
using namespace std;

/* App's message queue. */
extern MessageQueue<PlayOps> g_MsgQ;

HTTPPlayer::HTTPPlayer()
    : m_pPlaybin(NULL), m_isIntialised(false), m_sUrl("") {}

/* Create singlon HTTPPlayer instance and returned. */
HTTPPlayer *HTTPPlayer::getPlayerInstance() {
    static HTTPPlayer instance;
    return &instance;
}

// Virtual.
RETCODE
HTTPPlayer::load(const string &sUrl) {
    m_sUrl = sUrl;
    // clear pipeline
    if (m_pPlaybin != NULL) {
        gst_element_set_state(m_pPlaybin, GST_STATE_NULL);
        gst_object_unref(GST_OBJECT(m_pPlaybin));
        m_pPlaybin = NULL;
    }

    /* Create gstreamer elements */
    m_pPlaybin = gst_element_factory_make("playbin", NULL);
    if (m_pPlaybin == NULL) {
        std::cout << "ERROR:Playbin creation failed \n";
        return (INTERNAL_ERROR);
    }

    g_object_set(G_OBJECT(m_pPlaybin), "uri", m_sUrl.c_str(), NULL);
    return (SUCCESS);
}

RETCODE
HTTPPlayer::play() {
    GstStateChangeReturn ret =
        gst_element_set_state(m_pPlaybin, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        if (m_pPlaybin != NULL) {
            gst_element_set_state(m_pPlaybin, GST_STATE_NULL);
            gst_object_unref(GST_OBJECT(m_pPlaybin));
            m_pPlaybin = NULL;
        }
        return (PLAY_FAILED);
    }

    /* Add a _bus watch, so we get notified when a message arrives */
    _bus = gst_element_get_bus(m_pPlaybin);
    gst_bus_add_watch(_bus, (GstBusFunc)handleGstMsg, NULL);

    return (SUCCESS);
}

RETCODE
HTTPPlayer::pause() {
    GstState state;
    gst_element_get_state(m_pPlaybin, &state, 0, 0);
    if (state == GST_STATE_PLAYING) {
        gst_element_set_state(m_pPlaybin, GST_STATE_PAUSED);
    }

    return (SUCCESS);
}

RETCODE
HTTPPlayer::stop() {
    gst_element_set_state(m_pPlaybin, GST_STATE_READY);
    return (SUCCESS);
}

RETCODE
HTTPPlayer::init(int argc, char **argv) {
    if (m_isIntialised) {
        std::cout << "Already gstreamer library intialised. \n";
        return (SUCCESS);
    }

    gst_init(&argc, &argv);
    m_isIntialised = true;

    return (SUCCESS);
}

// Destructor.
HTTPPlayer::~HTTPPlayer() {
    // cout << "HTTPPlayer::~HTTPPlayer () destructor called \n";
}

/* Process messages from GStreamer */
gboolean HTTPPlayer::handleGstMsg(GstBus *_bus, GstMessage *msg) {
    GError *err;
    gchar *debug_info;

    switch (GST_MESSAGE_TYPE(msg)) {
    case GST_MESSAGE_ERROR:
        gst_message_parse_error(msg, &err, &debug_info);
        g_printerr("Error received from element %s: %s\n",
                   GST_OBJECT_NAME(msg->src), err->message);
        g_printerr("Debugging information: %s\n",
                   debug_info ? debug_info : "none");
        g_MsgQ.send(GSTERROR);
        g_clear_error(&err);
        g_free(debug_info);
        break;
    case GST_MESSAGE_EOS:
        g_print("End-Of-Stream reached.\n");
        break;
    }
    /* We want to keep receiving messages */
    return TRUE;
}