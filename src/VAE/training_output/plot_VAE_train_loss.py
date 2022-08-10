import pickle
import matplotlib.pyplot as plt
def loadData():
    # for reading also binary mode is important
    with open('loss_list_normalize.pkl', 'rb') as f:
        list_load_normalize = pickle.load(f)
    with open('loss_list_rotation.pkl', 'rb') as f:
        list_load_rotation = pickle.load(f)
    with open('loss_list_vertical_flip.pkl', 'rb') as f:
        list_load_vertical_flip = pickle.load(f)
    epoch_loss = [loss for i,loss in enumerate(list_load_normalize) if i%170==0]
    plt.plot(list_load_normalize, label='Batch normalised')
    plt.plot(list_load_rotation, label=r'Randomely rotated $[-45^\circ,45^\circ]$')
    plt.plot(list_load_vertical_flip, label='Vertically flipped($p=0.5$)')
    plt.xlabel('step')
    plt.ylabel('loss')
    plt.legend()
    plt.show()
    f.close()
loadData()